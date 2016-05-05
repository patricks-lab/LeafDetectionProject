/***
Graphic.java
Displays all the candidate leaves into a window.
The top is the most likely leaf, and the bottom is the least likely leaf.
***/
import java.awt.*;
import javax.swing.*;

public class graphic extends JPanel {
    public static int x = 0;
    public static ImageIcon[] ic;

    public void paintComponent(Graphics g)
    {
      super.paintComponent(g);
      Graphics2D g2d = (Graphics2D)g;
      for(int i = 0; i < x; i++)
      {
        //Draw each image, normalizing its height to 150 pixles per image.
        Image im = ic[i].getImage();
        int hei = 150;
        double wh = (double)im.getWidth(null)/(double)im.getHeight(null);
        int wid = (int)(150 * wh);
        g2d.drawImage(im,240,i*150,wid,hei,null);
      }

    }
    public static void main(String[] args)
    {
      JFrame f = new JFrame("Leaf Results");
      graphic panel = new graphic();

      x = args.length;
      ic = new ImageIcon[x];
      for(int i = 0; i < x; i++)
      {
        //Sets up new ImageIcons for each incoming image.
        ic[i] = new ImageIcon(args[i]);
      }

      f.setSize(600,1500);
      f.setLocationRelativeTo(null);
      f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
      f.add(panel);
      f.setVisible(true);
    }
}
