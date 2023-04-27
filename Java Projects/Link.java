/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Graph;

/**
 *
 * @author Elio
 */
public class Link {

    int key;
    double data;
    Link next;
    Link previous;

    public Link(int key, double data) {
        this.key = key;
        this.data = data;
        next = null;
    }

    public void displayLink() {
        System.out.print(key + ", " + data);
    }
}
