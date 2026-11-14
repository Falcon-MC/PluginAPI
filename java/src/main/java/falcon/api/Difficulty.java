package falcon.api;

import falcon.api.internal.FalconAbi;

public enum Difficulty {
    PEACEFUL(FalconAbi.DIFFICULTY_PEACEFUL),
    EASY(FalconAbi.DIFFICULTY_EASY),
    NORMAL(FalconAbi.DIFFICULTY_NORMAL),
    HARD(FalconAbi.DIFFICULTY_HARD);

    private final int mValue;

    Difficulty(int value) {
        mValue = value;
    }

    public int value() {
        return mValue;
    }

    public static Difficulty of(int value) {
        for (Difficulty entry : values()) {
            if (entry.mValue == value) {
                return entry;
            }
        }
        throw new IllegalArgumentException("Unknown value " + value);
    }
}
