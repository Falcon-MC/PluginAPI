using Falcon.Interop;

namespace Falcon
{
    public readonly record struct Vec3(double X, double Y, double Z)
    {
        public static Vec3 operator +(Vec3 left, Vec3 right)
        {
            return new Vec3(left.X + right.X, left.Y + right.Y, left.Z + right.Z);
        }

        public static Vec3 operator -(Vec3 left, Vec3 right)
        {
            return new Vec3(left.X - right.X, left.Y - right.Y, left.Z - right.Z);
        }

        public static Vec3 operator *(Vec3 value, double factor)
        {
            return new Vec3(value.X * factor, value.Y * factor, value.Z * factor);
        }

        internal static Vec3 From(FalconVec3 value)
        {
            return new Vec3(value.x, value.y, value.z);
        }

        internal FalconVec3 ToNative()
        {
            FalconVec3 value;
            value.x = X;
            value.y = Y;
            value.z = Z;
            return value;
        }
    }
}
