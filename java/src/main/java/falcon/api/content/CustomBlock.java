package falcon.api.content;

public final class CustomBlock {
    private final String mIdentifier;
    private String mDisplayName = "";
    private String mTexture = "";
    private String mCreativeCategory = "";
    private float mDestroyTime = 1.0f;
    private float mExplosionResistance = 1.0f;
    private int mLightEmission = 0;
    private float mFriction = 0.6f;
    private String mDrop = "";
    private BlockInteractHandler mOnInteract;
    private BlockBreakHandler mOnBreak;

    public CustomBlock(String identifier) {
        mIdentifier = identifier;
    }

    public String identifier() {
        return mIdentifier;
    }

    public String displayName() {
        return mDisplayName;
    }

    public CustomBlock displayName(String displayName) {
        mDisplayName = displayName;
        return this;
    }

    public String texture() {
        return mTexture;
    }

    public CustomBlock texture(String texture) {
        mTexture = texture;
        return this;
    }

    public String creativeCategory() {
        return mCreativeCategory;
    }

    public CustomBlock creativeCategory(String creativeCategory) {
        mCreativeCategory = creativeCategory;
        return this;
    }

    public float destroyTime() {
        return mDestroyTime;
    }

    public CustomBlock destroyTime(float destroyTime) {
        mDestroyTime = destroyTime;
        return this;
    }

    public float explosionResistance() {
        return mExplosionResistance;
    }

    public CustomBlock explosionResistance(float explosionResistance) {
        mExplosionResistance = explosionResistance;
        return this;
    }

    public int lightEmission() {
        return mLightEmission;
    }

    public CustomBlock lightEmission(int lightEmission) {
        mLightEmission = lightEmission;
        return this;
    }

    public float friction() {
        return mFriction;
    }

    public CustomBlock friction(float friction) {
        mFriction = friction;
        return this;
    }

    public String drop() {
        return mDrop;
    }

    public CustomBlock drop(String drop) {
        mDrop = drop;
        return this;
    }

    public BlockInteractHandler onInteract() {
        return mOnInteract;
    }

    public CustomBlock onInteract(BlockInteractHandler onInteract) {
        mOnInteract = onInteract;
        return this;
    }

    public BlockBreakHandler onBreak() {
        return mOnBreak;
    }

    public CustomBlock onBreak(BlockBreakHandler onBreak) {
        mOnBreak = onBreak;
        return this;
    }
}
