package falcon.api.command;

import falcon.api.internal.FalconAbi;

public enum CommandPermission {
    ANY(FalconAbi.PERMISSION_ANY),
    OPERATOR(FalconAbi.PERMISSION_OPERATOR);

    private final int mValue;

    CommandPermission(int value) {
        mValue = value;
    }

    public int value() {
        return mValue;
    }
}
