using Falcon.Interop;

namespace Falcon
{
    public enum PermissionDefault : uint
    {
        False = FalconConstants.PermissionDefaultFalse,
        True = FalconConstants.PermissionDefaultTrue,
        Operator = FalconConstants.PermissionDefaultOperator
    }
}
