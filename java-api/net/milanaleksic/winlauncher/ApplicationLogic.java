package net.milanaleksic.winlauncher;

import java.nio.channels.FileLock;

public abstract class ApplicationLogic {

    private FileLock lock = null;

    private final WinLauncherConfig config;

    public ApplicationLogic() {
        this.config = new WinLauncherConfig();
    }

    public ApplicationLogic(WinLauncherConfig config) {
        this.config = config;
    }

    public abstract void run() ;

    public abstract void couldNotRun(Exception e) ;

    void setFileLock(FileLock lock) {
        this.lock = lock;
    }

    public void restartApplication() {
        restartApplication(config.getPauseTime());
    }

    public void restartApplication(int pauseTimeMillis) {
        WinLauncherUtil.closeSingletonApplicationFileLock(lock);
        WinLauncherUtil.restartApplication(pauseTimeMillis, config.getLauncherLocation());
    }

}
