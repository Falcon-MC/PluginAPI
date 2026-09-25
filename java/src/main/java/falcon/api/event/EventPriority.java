package falcon.api.event;

import falcon.api.internal.FalconAbi;

public enum EventPriority {
    LOWEST(FalconAbi.PRIORITY_LOWEST),
    LOW(FalconAbi.PRIORITY_LOW),
    NORMAL(FalconAbi.PRIORITY_NORMAL),
    HIGH(FalconAbi.PRIORITY_HIGH),
    HIGHEST(FalconAbi.PRIORITY_HIGHEST),
    MONITOR(FalconAbi.PRIORITY_MONITOR);

    private final int mValue;

    EventPriority(int value) {
        mValue = value;
    }

    public int value() {
        return mValue;
    }
}
