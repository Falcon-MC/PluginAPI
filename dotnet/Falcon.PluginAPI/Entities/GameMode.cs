using Falcon.Interop;

namespace Falcon
{
    public enum GameMode : uint
    {
        Survival = FalconConstants.GameModeSurvival,
        Creative = FalconConstants.GameModeCreative,
        Adventure = FalconConstants.GameModeAdventure,
        Spectator = FalconConstants.GameModeSpectator
    }
}
