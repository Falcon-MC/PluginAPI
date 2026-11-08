package falcon.api;

import falcon.api.internal.FalconAbi;

/**
 * How an image meets a grid of another aspect ratio. CONTAIN shows the whole image with transparent margins, COVER
 * fills the grid and crops the overflow, STRETCH distorts it.
 */
public enum ImageFit {
    STRETCH(FalconAbi.IMAGE_FIT_STRETCH),
    CONTAIN(FalconAbi.IMAGE_FIT_CONTAIN),
    COVER(FalconAbi.IMAGE_FIT_COVER);

    private final int mValue;

    ImageFit(int value) {
        mValue = value;
    }

    public int value() {
        return mValue;
    }
}
