import net.milanaleksic.winlauncher.ApplicationLogic;
import net.milanaleksic.winlauncher.WinLauncherUtil;

import javax.swing.*;

class A {

    public static void main(final String[] args) {

        WinLauncherUtil.wrapSingletonApplicationLogic(new ApplicationLogic() {

            public void run() {
                B b = new B();
                String additionalMessage = "";
                if (args != null && args.length > 0)
                    additionalMessage = " and program param is: " + args[0];
                int result = JOptionPane.showConfirmDialog(null, b.getMessage() + additionalMessage +
                        "\r\n\r\nDo you wish to restart application using WinLauncher API?",
                        "WinLauncher Sample Application", JOptionPane.YES_NO_OPTION);
                if (result == JOptionPane.YES_OPTION)
                    restartApplication();
            }

            public void couldNotRun(Exception e) {
                JOptionPane.showMessageDialog(null, "Error - "+e.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
            }

        });

    }


}