package falcon.api.command;

import falcon.api.internal.FalconAbi;

public enum CommandParamType {
    INT(FalconAbi.COMMAND_PARAM_INT),
    FLOAT(FalconAbi.COMMAND_PARAM_FLOAT),
    STRING(FalconAbi.COMMAND_PARAM_STRING),
    BOOL(FalconAbi.COMMAND_PARAM_BOOL),
    TARGET(FalconAbi.COMMAND_PARAM_TARGET),
    POSITION(FalconAbi.COMMAND_PARAM_POSITION),
    BLOCK_POSITION(FalconAbi.COMMAND_PARAM_BLOCK_POSITION),
    MESSAGE(FalconAbi.COMMAND_PARAM_MESSAGE),
    JSON(FalconAbi.COMMAND_PARAM_JSON),
    ENUM(FalconAbi.COMMAND_PARAM_ENUM);

    private final int mValue;

    CommandParamType(int value) {
        mValue = value;
    }

    public int value() {
        return mValue;
    }
}
