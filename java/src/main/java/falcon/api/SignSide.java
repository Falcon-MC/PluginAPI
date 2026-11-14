package falcon.api;

import falcon.api.internal.FalconAbi;

public enum SignSide {
    FRONT(FalconAbi.SIGN_FRONT),
    BACK(FalconAbi.SIGN_BACK);

    private final int mValue;

    SignSide(int value) {
        mValue = value;
    }

    public int value() {
        return mValue;
    }
}
