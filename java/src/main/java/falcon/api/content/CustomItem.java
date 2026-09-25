package falcon.api.content;

public final class CustomItem {
    private final String mIdentifier;
    private String mDisplayName = "";
    private String mIcon = "";
    private String mCreativeCategory = "";
    private int mMaxStackSize = 64;
    private int mMaxDurability = 0;
    private boolean mHandEquipped = false;
    private ItemUseHandler mOnUse;
    private ItemUseOnBlockHandler mOnUseOnBlock;

    public CustomItem(String identifier) {
        mIdentifier = identifier;
    }

    public String identifier() {
        return mIdentifier;
    }

    public String displayName() {
        return mDisplayName;
    }

    public CustomItem displayName(String displayName) {
        mDisplayName = displayName;
        return this;
    }

    public String icon() {
        return mIcon;
    }

    public CustomItem icon(String icon) {
        mIcon = icon;
        return this;
    }

    public String creativeCategory() {
        return mCreativeCategory;
    }

    public CustomItem creativeCategory(String creativeCategory) {
        mCreativeCategory = creativeCategory;
        return this;
    }

    public int maxStackSize() {
        return mMaxStackSize;
    }

    public CustomItem maxStackSize(int maxStackSize) {
        mMaxStackSize = maxStackSize;
        return this;
    }

    public int maxDurability() {
        return mMaxDurability;
    }

    public CustomItem maxDurability(int maxDurability) {
        mMaxDurability = maxDurability;
        return this;
    }

    public boolean handEquipped() {
        return mHandEquipped;
    }

    public CustomItem handEquipped(boolean handEquipped) {
        mHandEquipped = handEquipped;
        return this;
    }

    public ItemUseHandler onUse() {
        return mOnUse;
    }

    public CustomItem onUse(ItemUseHandler onUse) {
        mOnUse = onUse;
        return this;
    }

    public ItemUseOnBlockHandler onUseOnBlock() {
        return mOnUseOnBlock;
    }

    public CustomItem onUseOnBlock(ItemUseOnBlockHandler onUseOnBlock) {
        mOnUseOnBlock = onUseOnBlock;
        return this;
    }
}
