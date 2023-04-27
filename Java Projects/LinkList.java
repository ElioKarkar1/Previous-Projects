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
class LinkList {

    Link first;

    public Link find(int vertex) {
        if (this.isEmpty()) {
            return null;
        }

        Link x = first;
        while (x.key != vertex) {
            x = x.next;
            if (x == null) {
                return null;
            }
        }

        return x;
    }

    public boolean isEmpty() {
        return first == null;
    }

    public void insertAfter(int lookupKey, int key, double data) {
        Link x = new Link(key, data);
        Link y = this.find(lookupKey);
        x.next = y.next;
        y.next = x;
    }

    public void insertFirst(int key, double data) {
        Link x = new Link(key, data);
        x.next = first;
        first = x;
    }

    public Link delete(int lookupKey) {
        Link x = first;
        Link y = null;
        while (x.key != lookupKey) {
            y = x;
            x = x.next;
            if (x == null) {
                return null;
            }
        }

        if (x == first) {
            first = first.next;
        } else {
            y.next = x.next;
        }

        return x;
    }

    public Link deleteFirst() {
        Link x = first;
        first = first.next;
        return x;
    }

    public void displayLinkList() {
        Link x = first;
        if (this.isEmpty()) {
            System.out.println("empty!");
            return;
        }

        x.displayLink();
        do {
            x = x.next;
            x.displayLink();
        } while (x.next != null);
        System.out.println();
    }

}
