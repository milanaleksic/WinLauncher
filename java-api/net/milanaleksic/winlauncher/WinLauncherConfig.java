package net.milanaleksic.winlauncher;

public class WinLauncherConfig {

    private static final String WIN_LAUNCHER_DEFAULT_EXECUTABLE = "WinLauncher.exe";

    private static final int WIN_LAUNCHER_DEFAULT_PAUSE_TIME = 5000;

    private String executable;

    private int pauseTime;

    private boolean debug;

    private boolean noChangeDir;

    public WinLauncherConfig(String executable, int pauseTime, boolean debug, boolean noChangeDir) {
        this.executable = executable;
        this.pauseTime = pauseTime;
        this.debug = debug;
        this.noChangeDir = noChangeDir;
    }

    public WinLauncherConfig() {
        this.executable = WIN_LAUNCHER_DEFAULT_EXECUTABLE;
        this.pauseTime = WIN_LAUNCHER_DEFAULT_PAUSE_TIME;
        this.debug = false;
        this.noChangeDir = false;
    }

    public String getExecutable() {
        return executable;
    }

    public void setExecutable(String executable) {
        this.executable = executable;
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
