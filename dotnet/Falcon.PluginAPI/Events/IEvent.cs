namespace Falcon
{
    public interface IEvent<TSelf> where TSelf : Event, IEvent<TSelf>
    {
        static abstract uint EventType { get; }

        static abstract TSelf Create(nint handle);
    }
}
