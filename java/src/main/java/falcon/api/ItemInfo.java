package falcon.api;

public record ItemInfo(String name, int maxStackSize, int maxDurability, int toolType, int toolTier,
                       int attackDamage, int armorSlot, int armorPoints, int armorToughness,
                       int nutrition, float saturation) {
}
