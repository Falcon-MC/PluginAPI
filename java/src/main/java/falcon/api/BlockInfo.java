package falcon.api;

public record BlockInfo(String name, float hardness, float blastResistance, int toolType, int toolTier,
                        int lightEmission, boolean transparent, boolean solid, int flameEncouragement,
                        int flammability, int maxStackSize) {
}
