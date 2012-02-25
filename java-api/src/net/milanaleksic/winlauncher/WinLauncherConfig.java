package net.milanaleksic.winlauncher;

public class WinLauncherConfig {

    private static final String WIN_LAUNCHER_DEFAULT_EXECUTABLE = "WinLauncher.exe";

    private static final int WIN_LAUNCHER_DEFAULT_PAUSE_TIME = 5000;

    private String executable;

    private int pauseTime;

    private boolean debug;

    private boolean noChangeDir;

    public static WinLauncherConfig build() {
        return new WinLauncherConfig();
    }

    private WinLauncherConfig() {
        this.executable = WIN_LAUNCHER_DEFAULT_EXECUTABLE;
        this.pauseTime = WIN_LAUNCHER_DEFAULT_PAUSE_TIME;
        this.debug = false;
        this.noChangeDir = false;
    }

    public String getExecutable() {
        return executable;
    }

    public WinLauncherConfig setExecutable(String executable) {
        this.executable = executable;
        return this;
    }

    public int getPauseTime() {
        return pauseTime;
    }

    public WinLauncherConfig setPauseTime(int pauseTime) {
        this.pauseTime = pauseTime;
        return this;
    }

    public boolean isDebug() {
        return debug;
    }

    public WinLauncherConfig setDebug(boolean debug) {
        this.debug = debug;
        return this;
    }

    public boolean isNoChangeDir() {
        return noChangeDir;
    }

    public WinLauncherConfig setNoChangeDir(boolean noChangeDir) {
        this.noChangeDir = noChangeDir;
        return this;
    }

    @Override
    public String toString() {
        return "WinLauncherConfig{" +
                "executable='" + executable + '\'' +
                ", pauseTime=" + pauseTime +
                ", debug=" + debug +
                ", noChangeDir=" + noChangeDir +
                '}';
    }
}
