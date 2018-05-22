import tkinter
import time
from PIL import Image, ImageTk, ImageSequence

class App:
    def __init__(self, parent):
        self.parent = parent
        self.entry = tkinter.Entry(parent)
        self.button = tkinter.Button(parent, text="What's your input?", command=self.on_button)
        self.button.pack()
        self.entry.pack()
        self.canvas = tkinter.Canvas(parent, width = 548, height = 610)
        self.canvas.pack()
    def animate(self,counter):
        self.canvas.itemconfig(self.image, image = self.sequence[counter])
        if not self.animating:
            return
        self.parent.after(33, lambda:self.animate((counter+1) % len(self.sequence)))
        if counter == 101:
            self.canvas.delete("all")
            self.animating = False
            return
        print(counter)
    def on_button(self):
        answer = self.entry.get()
        if answer == "1":
            self.sequence = [ImageTk.PhotoImage(img)
            for img in ImageSequence.Iterator(
                                              Image.open(
                                                         r'/Users/Arnold/Documents/Python_Scripts/giphy.gif'))]
            self.image = self.canvas.create_image(274, 305, image = self.sequence[0])
            self.animating = True
            self.animate(0)
        elif answer == "0":
            return

root = tkinter.Tk()
app = App(root)
root.mainloop()
