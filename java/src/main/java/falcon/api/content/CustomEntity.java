package falcon.api.content;

public final class CustomEntity {
    private final String mIdentifier;
    private float mWidth = 0.6f;
    private float mHeight = 1.8f;
    private float mMaxHealth = 20.0f;
    private boolean mSummonable = true;
    private EntityTickHandler mOnTick;
    private EntityInteractHandler mOnInteract;

    public CustomEntity(String identifier) {
        mIdentifier = identifier;
    }

    public String identifier() {
        return mIdentifier;
    }

    public float width() {
        return mWidth;
    }

    public CustomEntity width(float width) {
        mWidth = width;
        return this;
    }

    public float height() {
        return mHeight;
    }

    public CustomEntity height(float height) {
        mHeight = height;
        return this;
    }

    public float maxHealth() {
        return mMaxHealth;
    }

    public CustomEntity maxHealth(float maxHealth) {
        mMaxHealth = maxHealth;
        return this;
    }

    public boolean summonable() {
        return mSummonable;
    }

    public CustomEntity summonable(boolean summonable) {
        mSummonable = summonable;
        return this;
    }

    public EntityTickHandler onTick() {
        return mOnTick;
    }

    public CustomEntity onTick(EntityTickHandler onTick) {
        mOnTick = onTick;
        return this;
    }

    public EntityInteractHandler onInteract() {
        return mOnInteract;
    }

    public CustomEntity onInteract(EntityInteractHandler onInteract) {
        mOnInteract = onInteract;
        return this;
    }
}
