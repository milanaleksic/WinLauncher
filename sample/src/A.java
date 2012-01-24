import javax.swing.*;

class A {

    public static void main(String[] args) {
        B b = new B();
        String additionalMessage = "";
        if (args != null && args.length>0)
            additionalMessage = " and program param is: "+args[0];
        JOptionPane.showMessageDialog(null, b.getMessage()+additionalMessage);
    }

}