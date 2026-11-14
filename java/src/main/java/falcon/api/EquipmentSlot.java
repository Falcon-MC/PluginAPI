package falcon.api;

import falcon.api.internal.FalconAbi;

public enum EquipmentSlot {
    MAINHAND(FalconAbi.EQUIPMENT_MAINHAND),
    OFFHAND(FalconAbi.EQUIPMENT_OFFHAND),
    HEAD(FalconAbi.EQUIPMENT_HEAD),
    CHEST(FalconAbi.EQUIPMENT_CHEST),
    LEGS(FalconAbi.EQUIPMENT_LEGS),
    FEET(FalconAbi.EQUIPMENT_FEET),
    BODY(FalconAbi.EQUIPMENT_BODY);

    private final int mValue;

    EquipmentSlot(int value) {
        mValue = value;
    }

    public int value() {
        return mValue;
    }
}
