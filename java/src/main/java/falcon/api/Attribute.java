package falcon.api;

import falcon.api.internal.FalconAbi;

import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;

public record Attribute(String name, float value, float minimum, float maximum, float defaultValue) {
    static Attribute read(String name, MemorySegment segment) {
        return new Attribute(name, segment.get(ValueLayout.JAVA_FLOAT, FalconAbi.Attribute.VALUE),
                segment.get(ValueLayout.JAVA_FLOAT, FalconAbi.Attribute.MINIMUM),
                segment.get(ValueLayout.JAVA_FLOAT, FalconAbi.Attribute.MAXIMUM),
                segment.get(ValueLayout.JAVA_FLOAT, FalconAbi.Attribute.DEFAULT_VALUE));
    }
}
