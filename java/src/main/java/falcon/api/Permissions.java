package falcon.api;

import falcon.api.internal.Interop;
import falcon.api.internal.PluginContext;

import java.lang.foreign.Arena;

public final class Permissions {
    private final PluginContext mContext;

    Permissions(PluginContext context) {
        mContext = context;
    }

    public boolean add(String node) {
        return add(node, PermissionDefault.OPERATOR);
    }

    public boolean add(String node, PermissionDefault defaultValue) {
        try (Arena arena = Arena.ofConfined()) {
            return Interop.api().registerPermission(mContext.handle(), Interop.text(arena, node),
                    defaultValue.value()) != 0;
        }
    }
}
