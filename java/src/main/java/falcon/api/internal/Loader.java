package falcon.api.internal;

import falcon.api.Plugin;

import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;
import java.lang.invoke.MethodHandles;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.net.URL;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public final class Loader {
    private static final MethodHandles.Lookup LOOKUP = MethodHandles.lookup();
    private static final MemorySegment ON_LOAD =
            Upcalls.stub(LOOKUP, "onLoad", FalconAbi.PluginCallbacks.ON_LOAD_FUNCTION);
    private static final MemorySegment ON_ENABLE =
            Upcalls.stub(LOOKUP, "onEnable", FalconAbi.PluginCallbacks.ON_ENABLE_FUNCTION);
    private static final MemorySegment ON_DISABLE =
            Upcalls.stub(LOOKUP, "onDisable", FalconAbi.PluginCallbacks.ON_DISABLE_FUNCTION);
    private static final Map<String, PluginClassLoader> LOADERS = new ConcurrentHashMap<>();

    private Loader() {
    }

    public static boolean load(long api, long plugin, long callbacks, String jarPath, String mainClass,
                               String dependencies) {
        NativeApi nativeApi = bind(api);
        MemorySegment handle = MemorySegment.ofAddress(plugin);
        PluginContext context = new PluginContext(handle, Interop.string(nativeApi.pluginName(handle)));

        Thread thread = Thread.currentThread();
        ClassLoader previous = thread.getContextClassLoader();
        try {
            URL jar = Path.of(jarPath).toUri().toURL();
            PluginClassLoader classLoader = new PluginClassLoader(context.name(), jar, Loader.class.getClassLoader(),
                    dependencyLoaders(dependencies));
            thread.setContextClassLoader(classLoader);
            context.attach(classLoader, null);

            Plugin instance = instantiate(context, classLoader, mainClass);
            context.attach(classLoader, instance);
            fillCallbacks(callbacks, context);
            LOADERS.put(key(context.name()), classLoader);
            return true;
        } catch (InvocationTargetException exception) {
            Throwable cause = exception.getCause() == null ? exception : exception.getCause();
            context.report("The constructor of " + mainClass + " threw an exception", cause);
            return false;
        } catch (ClassNotFoundException exception) {
            context.report("The main class " + mainClass + " is not in " + jarPath, exception);
            return false;
        } catch (Throwable throwable) {
            context.report("Could not create " + mainClass, throwable);
            return false;
        } finally {
            thread.setContextClassLoader(previous);
        }
    }

    private static NativeApi bind(long address) {
        long expected = FalconAbi.ServerApi.LAYOUT.byteSize();
        MemorySegment table = MemorySegment.ofAddress(address).reinterpret(expected);
        long size = Integer.toUnsignedLong(table.get(ValueLayout.JAVA_INT, FalconAbi.ServerApi.SIZE));
        int major = table.get(ValueLayout.JAVA_INT, FalconAbi.ServerApi.VERSION_MAJOR);
        int minor = table.get(ValueLayout.JAVA_INT, FalconAbi.ServerApi.VERSION_MINOR);
        if (major != FalconAbi.API_VERSION_MAJOR || size < expected) {
            throw new IllegalStateException("the Java plugin API is built for API " + FalconAbi.API_VERSION_MAJOR + "."
                                            + FalconAbi.API_VERSION_MINOR + ", the server provides " + major + "."
                                            + minor);
        }
        return Interop.bind(table);
    }

    private static List<PluginClassLoader> dependencyLoaders(String dependencies) {
        List<PluginClassLoader> loaders = new ArrayList<>();
        for (String name : dependencies.split("\n")) {
            PluginClassLoader loader = LOADERS.get(key(name));
            if (loader != null && !loaders.contains(loader)) {
                loaders.add(loader);
            }
        }
        return loaders;
    }

    private static Plugin instantiate(PluginContext context, ClassLoader classLoader, String mainClass)
            throws ReflectiveOperationException {
        Class<?> type = Class.forName(mainClass, true, classLoader);
        if (!Plugin.class.isAssignableFrom(type)) {
            throw new IllegalStateException(mainClass + " does not extend " + Plugin.class.getName());
        }

        Constructor<? extends Plugin> constructor = type.asSubclass(Plugin.class).getDeclaredConstructor();
        constructor.setAccessible(true);
        PluginContext.offer(context);
        try {
            return constructor.newInstance();
        } finally {
            PluginContext.withdraw();
        }
    }

    private static void fillCallbacks(long address, PluginContext context) {
        MemorySegment callbacks = MemorySegment.ofAddress(address)
                .reinterpret(FalconAbi.PluginCallbacks.LAYOUT.byteSize());
        callbacks.set(ValueLayout.ADDRESS, FalconAbi.PluginCallbacks.ON_LOAD, ON_LOAD);
        callbacks.set(ValueLayout.ADDRESS, FalconAbi.PluginCallbacks.ON_ENABLE, ON_ENABLE);
        callbacks.set(ValueLayout.ADDRESS, FalconAbi.PluginCallbacks.ON_DISABLE, ON_DISABLE);
        callbacks.set(ValueLayout.ADDRESS, FalconAbi.PluginCallbacks.USER_DATA, Upcalls.register(context));
    }

    private static String key(String name) {
        return name.trim().toLowerCase(Locale.ROOT);
    }

    private static void onLoad(MemorySegment userData) {
        try {
            PluginContext context = Upcalls.target(userData, PluginContext.class);
            if (context != null) {
                context.guard("onLoad threw an exception", () -> context.plugin().onLoad());
            }
        } catch (Throwable ignored) {
        }
    }

    private static int onEnable(MemorySegment userData) {
        try {
            PluginContext context = Upcalls.target(userData, PluginContext.class);
            if (context == null) {
                return 0;
            }
            return context.test("onEnable threw an exception", () -> context.plugin().onEnable()) ? 1 : 0;
        } catch (Throwable ignored) {
            return 0;
        }
    }

    private static void onDisable(MemorySegment userData) {
        try {
            PluginContext context = Upcalls.target(userData, PluginContext.class);
            if (context != null) {
                context.guard("onDisable threw an exception", () -> context.plugin().onDisable());
            }
        } catch (Throwable ignored) {
        }
    }
}
