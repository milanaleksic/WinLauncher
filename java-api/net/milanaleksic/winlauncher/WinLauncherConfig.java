package net.milanaleksic.winlauncher;

public class WinLauncherConfig {

    private static final String WIN_LAUNCHER_DEFAULT_EXECUTABLE = "WinLauncher.exe";

    private static final int WIN_LAUNCHER_DEFAULT_PAUSE_TIME = 5000;

    private String launcherLocation;

    private int pauseTime;

    private boolean debug;

    private boolean noChangeDir;

    public WinLauncherConfig(String launcherLocation, int pauseTime, boolean debug, boolean noChangeDir) {
        this.launcherLocation = launcherLocation;
        this.pauseTime = pauseTime;
        this.debug = debug;
        this.noChangeDir = noChangeDir;
    }

    public WinLauncherConfig() {
        this.launcherLocation = WIN_LAUNCHER_DEFAULT_EXECUTABLE;
        this.pauseTime = WIN_LAUNCHER_DEFAULT_PAUSE_TIME;
        this.debug = false;
        this.noChangeDir = false;
    }

    public String getLauncherLocation() {
        return launcherLocation;
    }

    public void setLauncherLocation(String launcherLocation) {
        this.launcherLocation = launcherLocation;
    }

    public int getPauseTime() {
        return pauseTime;
    }

    public void setPauseTime(int pauseTime) {
        this.pauseTime = pauseTime;
    }

    public boolean isDebug() {
        return debug;
    }

    public void setDebug(boolean debug) {
        this.debug = debug;
    }

    public boolean isNoChangeDir() {
        return noChangeDir;
    }

    public void setNoChangeDir(boolean noChangeDir) {
        this.noChangeDir = noChangeDir;
    }
}
