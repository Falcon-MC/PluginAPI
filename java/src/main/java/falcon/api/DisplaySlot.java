package falcon.api;

import falcon.api.internal.FalconAbi;

public enum DisplaySlot {
    SIDEBAR(FalconAbi.DISPLAY_SLOT_SIDEBAR),
    LIST(FalconAbi.DISPLAY_SLOT_LIST),
    BELOW_NAME(FalconAbi.DISPLAY_SLOT_BELOW_NAME);

    private final int mValue;

    DisplaySlot(int value) {
        mValue = value;
    }

    public int value() {
        return mValue;
    }
}
