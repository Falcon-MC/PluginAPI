using Falcon.Interop;

namespace Falcon
{
    public enum CommandPermission : uint
    {
        Any = FalconConstants.PermissionAny,
        Operator = FalconConstants.PermissionOperator
    }
}
