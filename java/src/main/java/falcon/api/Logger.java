package falcon.api;

import falcon.api.internal.FalconAbi;
import falcon.api.internal.PluginContext;

public final class Logger {
    private final PluginContext mContext;

    Logger(PluginContext context) {
        mContext = context;
    }

    public void info(String message) {
        mContext.log(FalconAbi.LOG_INFO, message);
    }

    public void warning(String message) {
        mContext.log(FalconAbi.LOG_WARNING, message);
    }

    public void error(String message) {
        mContext.log(FalconAbi.LOG_ERROR, message);
    }

    public void error(String message, Throwable throwable) {
        mContext.report(message, throwable);
    }
}
