package falcon.api.internal;

import java.net.URL;
import java.net.URLClassLoader;
import java.util.List;

final class PluginClassLoader extends URLClassLoader {
    static {
        ClassLoader.registerAsParallelCapable();
    }

    private final List<PluginClassLoader> mDependencies;

    PluginClassLoader(String name, URL jar, ClassLoader parent, List<PluginClassLoader> dependencies) {
        super(name, new URL[]{jar}, parent);
        mDependencies = List.copyOf(dependencies);
    }

    @Override
    protected Class<?> loadClass(String name, boolean resolve) throws ClassNotFoundException {
        synchronized (getClassLoadingLock(name)) {
            Class<?> type = findLoadedClass(name);
            if (type == null) {
                type = fromParent(name);
            }
            if (type == null) {
                type = fromDependencies(name);
            }
            if (type == null) {
                type = findClass(name);
            }
            if (resolve) {
                resolveClass(type);
            }
            return type;
        }
    }

    private Class<?> fromParent(String name) {
        try {
            return getParent().loadClass(name);
        } catch (ClassNotFoundException exception) {
            return null;
        }
    }

    private Class<?> fromDependencies(String name) {
        for (PluginClassLoader dependency : mDependencies) {
            Class<?> type = dependency.exported(name);
            if (type != null) {
                return type;
            }
        }
        return null;
    }

    private Class<?> exported(String name) {
        synchronized (getClassLoadingLock(name)) {
            Class<?> type = findLoadedClass(name);
            if (type != null) {
                return type;
            }
            try {
                return findClass(name);
            } catch (ClassNotFoundException exception) {
                return fromDependencies(name);
            }
        }
    }
}
