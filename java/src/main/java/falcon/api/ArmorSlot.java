package falcon.api;

import falcon.api.internal.FalconAbi;

public enum ArmorSlot {
    HEAD(FalconAbi.ARMOR_HEAD),
    CHEST(FalconAbi.ARMOR_CHEST),
    LEGS(FalconAbi.ARMOR_LEGS),
    FEET(FalconAbi.ARMOR_FEET);

    private final int mValue;

    ArmorSlot(int value) {
        mValue = value;
    }

    public int value() {
        return mValue;
    }

    public static ArmorSlot of(int value) {
        for (ArmorSlot slot : values()) {
            if (slot.mValue == value) {
                return slot;
            }
        }
        throw new IllegalArgumentException("Unknown armor slot " + value);
    }
}
