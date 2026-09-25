package falcon.api;

import falcon.api.internal.FalconAbi;

public enum GameMode {
    SURVIVAL(FalconAbi.GAME_MODE_SURVIVAL),
    CREATIVE(FalconAbi.GAME_MODE_CREATIVE),
    ADVENTURE(FalconAbi.GAME_MODE_ADVENTURE),
    SPECTATOR(FalconAbi.GAME_MODE_SPECTATOR);

    private final int mValue;

    GameMode(int value) {
        mValue = value;
    }

    public int value() {
        return mValue;
    }

    public static GameMode of(int value) {
        for (GameMode gameMode : values()) {
            if (gameMode.mValue == value) {
                return gameMode;
            }
        }
        throw new IllegalArgumentException("Unknown game mode " + value);
    }
}
