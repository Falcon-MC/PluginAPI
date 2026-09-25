using System;
using Falcon.Interop;

namespace Falcon
{
    public readonly record struct BlockPos(int X, int Y, int Z)
    {
        public Vec3 Center
        {
            get
            {
                return new Vec3(X + 0.5, Y + 0.5, Z + 0.5);
            }
        }

        public int ChunkX
        {
            get
            {
                return X >> 4;
            }
        }

        public int ChunkZ
        {
            get
            {
                return Z >> 4;
            }
        }

        public static BlockPos Floor(Vec3 position)
        {
            return new BlockPos((int)Math.Floor(position.X), (int)Math.Floor(position.Y), (int)Math.Floor(position.Z));
        }

        public BlockPos Offset(int deltaX, int deltaY, int deltaZ)
        {
            return new BlockPos(X + deltaX, Y + deltaY, Z + deltaZ);
        }

        internal static BlockPos From(FalconBlockPos value)
        {
            return new BlockPos(value.x, value.y, value.z);
        }

        internal FalconBlockPos ToNative()
        {
            FalconBlockPos value;
            value.x = X;
            value.y = Y;
            value.z = Z;
            return value;
        }
    }
}
