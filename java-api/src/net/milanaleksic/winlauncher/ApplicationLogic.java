package net.milanaleksic.winlauncher;

import java.nio.channels.FileLock;

public abstract class ApplicationLogic {

    private FileLock lock = null;

    private final WinLauncherConfig config;

    public ApplicationLogic() {
        this.config = WinLauncherConfig.build();
    }

    public ApplicationLogic(WinLauncherConfig config) {
        this.config = config;
    }

    public abstract void run() ;

    public abstract void couldNotRun(Exception e) ;

    // this method is package-visible to allow only WinLauncherUtil to set the file lock member variable
    void setFileLock(FileLock lock) {
        this.lock = lock;
    }

    public void restartApplication() {
        WinLauncherUtil.closeSingletonApplicationFileLock(lock);
        WinLauncherUtil.restartApplication(config);
    }

}
