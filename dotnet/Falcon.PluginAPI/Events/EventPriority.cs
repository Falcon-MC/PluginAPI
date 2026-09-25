using Falcon.Interop;

namespace Falcon
{
    public enum EventPriority : uint
    {
        Lowest = FalconConstants.PriorityLowest,
        Low = FalconConstants.PriorityLow,
        Normal = FalconConstants.PriorityNormal,
        High = FalconConstants.PriorityHigh,
        Highest = FalconConstants.PriorityHighest,
        Monitor = FalconConstants.PriorityMonitor
    }
}
