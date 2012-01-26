package net.milanaleksic.winlauncher;

import java.nio.channels.FileLock;

public abstract class ApplicationLogic {

    private FileLock lock = null;

    public abstract void run() ;

    public abstract void couldNotRun(Exception e) ;

    void setFileLock(FileLock lock) {
        this.lock = lock;
    }

    public void restartApplication() {
        restartApplication(WinLauncherUtil.WIN_LAUNCHER_DEFAULT_PAUSE_TIME);
    }

    public void restartApplication(int pauseTimeMillis) {
        WinLauncherUtil.closeSingletonApplicationFileLock(lock);
        WinLauncherUtil.restartApplication(pauseTimeMillis);
    }

}
