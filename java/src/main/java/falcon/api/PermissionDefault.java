package falcon.api;

import falcon.api.internal.FalconAbi;

public enum PermissionDefault {
    FALSE(FalconAbi.PERMISSION_DEFAULT_FALSE),
    TRUE(FalconAbi.PERMISSION_DEFAULT_TRUE),
    OPERATOR(FalconAbi.PERMISSION_DEFAULT_OPERATOR);

    private final int mValue;

    PermissionDefault(int value) {
        mValue = value;
    }

    public int value() {
        return mValue;
    }
}
