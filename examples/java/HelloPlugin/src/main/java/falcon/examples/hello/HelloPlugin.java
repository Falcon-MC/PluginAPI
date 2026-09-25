package falcon.examples.hello;

import falcon.api.Plugin;
import falcon.examples.hello.commands.GiveDiamondCommand;
import falcon.examples.hello.commands.JoinsCommand;
import falcon.examples.hello.content.ThunderWand;
import falcon.examples.hello.listeners.BlockBreakListener;
import falcon.examples.hello.listeners.ChatListener;
import falcon.examples.hello.listeners.JoinListener;
import falcon.examples.hello.storage.JoinStore;
import falcon.examples.hello.tasks.AnnouncementTask;

public final class HelloPlugin extends Plugin {
    private final JoinStore mJoins = new JoinStore();

    @Override
    public void onLoad() {
        ThunderWand.registerTo(this);
    }

    @Override
    public boolean onEnable() {
        mJoins.load(dataFolder().resolve("joins.txt"));

        JoinListener.registerTo(this, mJoins);
        ChatListener.registerTo(this);
        BlockBreakListener.registerTo(this);
        JoinsCommand.registerTo(this, mJoins);
        GiveDiamondCommand.registerTo(this);
        AnnouncementTask.start(this);

        logger().info("HelloPlugin enabled");
        return true;
    }

    @Override
    public void onDisable() {
        mJoins.save();
        logger().info("HelloPlugin disabled");
    }
}
