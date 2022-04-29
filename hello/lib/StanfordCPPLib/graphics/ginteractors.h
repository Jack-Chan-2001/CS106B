/*
 * File: ginteractors.h
 * --------------------
 * This file exports a hierarchy of graphical interactors similar to those
 * provided in the Java Swing libraries.
 * <include src="pictures/ClassHierarchies/GInteractorHierarchy-h.html">
 * 
 * @version 2016/11/26
 * - added GInteractor::setForeground as alias for setColor
 * @version 2016/11/02
 * - added GTextField constructor that takes a string parameter
 * @version 2016/10/25
 * - added valueIsDouble/Integer/Real to GTextField
 * @version 2016/10/24
 * - added get/setFont, get/setMnemonic, get/setAccelerator to GInteractor
 * @version 2016/10/23
 * - added add/removeActionListener to GInteractor
 * @version 2016/10/16
 * - added setTooltip to interactors
 * - alphabetized methods
 * @version 2016/10/15
 * - added setPlaceholder method to GTextField
 * @version 2016/09/27
 * - added setText method to GButton, GCheckBox, GRadioButton
 * @version 2016/07/07
 * - added getText method to GButton, GCheckBox, GRadioButton
 * @version 2015/06/20
 * - added GRadioButton class
 * @version 2014/10/31
 * - added get/setIcon to GInteractor
 */

#ifndef _ginteractors_h
#define _ginteractors_h

#include "gobjects.h"
#include <graphics/gtypes.h>
#include <graphics/gwindow.h>
#include <collections/vector.h>

/*
 * Constants for alignments and icon positions.
 */
enum SwingConstants {
    SWING_CENTER = 0,
    SWING_TOP = 1,
    SWING_LEFT = 2,
    SWING_BOTTOM = 3,
    SWING_RIGHT = 4
};

/*
 * Class: GInteractor
 * ------------------
 * This abstract class is the superclass for all graphical interactors.
 * In most applications, interactors will be added to a control strip
 * along one of the sides of the <code>GWindow</code>, but they can
 * also be placed in specific positions just like any other
 * <code>GObject</code>.
 * <include src="pictures/ClassHierarchies/GInteractorHierarchy.html">
 */

class GInteractor : public GObject {
public:
    /*
     * Method: addActionListener
     * Usage: interactor.addActionListener();
     * --------------------------------------
     * Instructs the back-end to generate an ActionEvent when the user
     * interacts with this interactor.
     * You do not need to do this for buttons; they automatically receive
     * action events when they are clicked.
     * Calling this on a check box, radio button, or text field does have
     * an effect, though.
     */
    void addActionListener();

    // TODO: getAccelerator?

    /*
     * Method: getActionCommand
     * Usage: string cmd = interactor.getActionCommand();
     * --------------------------------------------------
     * Returns the action command associated with the interactor.
     */
    std::string getActionCommand() const;

    /* Prototypes for the virtual methods */
    virtual GRectangle getBounds() const;

    /*
     * Returns the font used by this interactor.
     */
    std::string getFont() const;

    /*
     * Methods related to get/setting icons on graphical interactors.
     */
    virtual std::string getIcon() const;

    /*
     * Returns the mnemonic (underlined character) used by this interactor,
     * or '\0' if none.
     */
    char getMnemonic() const;

    /*
     * Returns whether the interactor is enabled (true) or disabled (false).
     * If an interactor is disabled, it is grayed out onscreen and cannot be
     * clicked on to interact with it.
     * Interactors are enabled by default when first created.
     */
    bool isEnabled() const;

    /*
     * Method: removeActionListener
     * Usage: interactor.removeActionListener();
     * --------------------------------------
     * Instructs the back-end not to generate an ActionEvent when the user
     * interacts with this interactor.
     * You do not need to do this for buttons; they automatically receive
     * action events when they are clicked.
     * Calling this on a check box, radio button, or text field does have
     * an effect, though.
     */
    virtual void removeActionListener();

    /*
     * Method: requestFocus
     * Usage: interactor.requestFocus();
     * ---------------------------------
     * Asks for this control to receive keyboard input focus.
     */
    void requestFocus();

    /*
     * Sets a hotkey associated with this interactor.
     * Accelerator string is in a format such as "Shift-Ctrl-K".
     */
    void setAccelerator(const std::string& accelerator);

    /*
     * Method: setActionCommand
     * Usage: interactor.setActionCommand(cmd);
     * ----------------------------------------
     * Sets the action command to the indicated string.  If the string is not
     * empty, activating the interactor generates a <code>GActionEvent</code>.
     */
    void setActionCommand(const std::string& cmd);

    /*
     * See GObject::setBackground and setColor.
     */
    void setBackground(int rgb);
    void setBackground(const std::string& color);

    /*
     * Method: setBounds
     * Usage: interactor.setBounds(rect);
     *        interactor.setBounds(x, y, width, height);
     * -------------------------------------------------
     * Changes the bounds of the interactor to the specified values.
     */
    void setBounds(double x, double y, double width, double height);
    void setBounds(const GRectangle& size);

    /*
     * Sets the interactor to be enabled (true) or disabled (false).
     * If an interactor is disabled, it is grayed out onscreen and cannot be
     * clicked on to interact with it.
     * Interactors are enabled by default when first created.
     */
    void setEnabled(bool value);

    /*
     * These are aliases for GObject::setColor.
     */
    void setForeground(int rgb);
    void setForeground(const std::string& color);

    /*
     * Sets the font used on this interactor.
     * Font is in a format such as "Arial-Bold-16".
     */
    void setFont(const std::string& font);

    /*
     * Methods related to get/setting icons on graphical interactors.
     */
    virtual void setIcon(const std::string& filename);

    /*
     * Sets an underlined character to be used as a hotkey on this interactor,
     * or '\0' to remove any such mnemonic.
     */
    void setMnemonic(char mnemonic);

    /*
     * Method: setSize
     * Usage: interactor.setSize(size);
     *        interactor.setSize(width, height);
     * -----------------------------------------
     * Changes the size of the interactor to the specified width and height.
     */
    void setSize(double width, double height);
    void setSize(const GDimension& size);

    /*
     * Methods related to get/setting icons on graphical interactors.
     */
    virtual void setTextPosition(SwingConstants horizontal, SwingConstants vertical);

    /*
     * Sets tooltip text that will pop up on the interactor on mouse hover.
     */
    void setTooltip(const std::string& tooltipText);

protected:
    GInteractor();
    std::string actionCommand;
    std::string icon;
};

/*
 * Class: GButton
 * --------------
 * This interactor subclass represents an onscreen button.  The following
 * program displays a button that, when pressed, generates the message
 * &ldquo;Please do not press this button again&rdquo;
 * (with thanks to Douglas Adams&rsquo;s <i>Hitchhiker&rsquo;s
 * Guide to the Galaxy</i>):
 *
 *<pre>
 *    int main() {
 *       GWindow gw;
 *       GButton *button = new GButton("RED");
 *       gw.addToRegion(button, "SOUTH");
 *       while (true) {
 *          GEvent e = waitForEvent(ACTION_EVENT | CLICK_EVENT);
 *          if (e.getEventType() == MOUSE_CLICKED) break;
 *          cout << "Please do not press this button again." << endl;
 *       }
 *       return 0;
 *    }
 *</pre>
 */

class GButton : public GInteractor {

public:
    /*
     * Constructor: GButton
     * Usage: GButton *button = new GButton(label);
     * --------------------------------------------
     * Creates a <code>GButton</code> with the specified label.  This
     * constructor also sets the action command for the button to the
     * label string.
     * If the label is omitted, uses an empty string.
     */
    GButton(std::string label = "");

    /*
     * Returns the text label showing on the button.
     */
    virtual std::string getText() const;

    /*
     * Sets the text label showing on the button to be the given text.
     */
    virtual void setText(std::string text);

    /* Prototypes for the virtual methods */
    virtual std::string getType() const;
    virtual std::string toString() const;

private:
    std::string label;
};

/*
 * Class: GCheckBox
 * ----------------
 * This interactor subclass represents an onscreen check box.  Clicking
 * once on the check box selects it; clicking again removes the selection.
 * If a <code>GCheckBox</code> has an action command, clicking on the box
 * generates a <code>GActionEvent</code>.
 * <include src="pictures/GInteractorDiagrams/GCheckBox.html">
 */
class GCheckBox : public GInteractor {

public:

    /*
     * Constructor: GCheckBox
     * Usage: GCheckBox *chkbox = new GCheckBox(label);
     * ------------------------------------------------
     * Creates a <code>GCheckBox</code> with the specified label.  In contrast
     * to the <code>GButton</code> constructor, this constructor does not set
     * an action command.
     */
    GCheckBox(std::string label = "");

    /*
     * Returns the text label showing on the button.
     */
    virtual std::string getText() const;

    /*
     * Sets the text label showing on the button to be the given text.
     */
    virtual void setText(std::string text);

    /*
     * Method: setSelected
     * Usage: chkbox->setSelected(state);
     * ----------------------------------
     * Sets the state of the check box.
     */
    void setSelected(bool state);
    void setChecked(bool state);

    /*
     * Method: isSelected
     * Usage: if (chkbox->isSelected()) ...
     * ------------------------------------
     * Returns <code>true</code> if the check box is selected.
     */
    bool isSelected();
    bool isChecked();

    /* Prototypes for the virtual methods */
    virtual std::string getType() const;
    virtual std::string toString() const;

private:
    std::string label;
};

/*
 * Class: GRadioButton
 * -------------------
 * This interactor subclass represents an onscreen radio button.  Clicking
 * once on the radio button selects it and deselects others in its group.
 * If a <code>GRadioButton</code> has an action command, clicking on the box
 * generates a <code>GActionEvent</code>.
 */
class GRadioButton : public GInteractor {
public:
    /*
     * Constructor: GRadioButton
     * Usage: GRadioButton* button = new GRadioButton(label, group);
     * -------------------------------------------------------------
     * Creates a <code>GRadioButton</code> with the specified label.  In contrast
     * to the <code>GButton</code> constructor, this constructor does not set
     * an action command.
     * All radio buttons must be part of a named group.
     * Only one radio button within a given group can be selected at any time.
     * If no group name is provided, the button is placed into a default group.
     * Button is not initially selected unless 'selected' of true is passed.
     */
    GRadioButton(std::string label = "", std::string group = "default", bool selected = false);

    /*
     * Returns the text label showing on the button.
     */
    virtual std::string getText() const;

    /*
     * Sets the text label showing on the button to be the given text.
     */
    virtual void setText(std::string text);

    /*
     * Method: setSelected
     * Usage: button->setSelected(state);
     * ----------------------------------
     * Sets the state of the radio button.
     */
    void setSelected(bool state);
    void setChecked(bool state);

    /*
     * Method: isSelected
     * Usage: if (button->isSelected()) ...
     * ------------------------------------
     * Returns <code>true</code> if the button is selected.
     */
    bool isSelected();
    bool isChecked();

    /*
     * Method: getGroup
     * Usage: string group = button->getGroup();
     * -----------------------------------------
     * Returns the name of the button group to which this radio button belongs.
     */
    std::string getGroup() const;

    /* Prototypes for the virtual methods */
    virtual std::string getType() const;
    virtual std::string toString() const;

private:
    std::string group;
    std::string label;
};

/*
 * Class: GSlider
 * --------------
 * This interactor subclass represents an onscreen slider.  Dragging
 * the slider control generates an <code>ActionEvent</code> if the
 * slider has a nonempty action command.
 * <include src="pictures/GInteractorDiagrams/GSlider.html">
 */

class GSlider : public GInteractor {

public:

    /*
     * Constructor: GSlider
     * Usage: GSlider *slider = new GSlider();
     *        GSlider *slider = new GSlider(min, max, value);
     * ------------------------------------------------------
     * Creates a horizontal <code>GSlider</code>.  The second form allows
     * the client to specify the minimum value, maximum value, and current
     * value of the slider.  The first form is equivalent to calling
     * <code>GSlider(0, 100, 50)</code>.  Assigning an action command
     * to the slider causes the slider to generate an action event whenever
     * the slider value changes.
     */
    GSlider();
    GSlider(int min, int max, int value);

    /*
     * Method: getValue
     * Usage: int value = slider->getValue();
     * --------------------------------------
     * Returns the current value of the slider.
     */
    int getValue();

    /*
     * Method: setValue
     * Usage: slider->setValue(value);
     * -------------------------------
     * Sets the current value of the slider.
     */
    void setValue(int value);
    
    int getMajorTickSpacing() const;
    int getMinorTickSpacing() const;
    bool getPaintLabels() const;
    bool getPaintTicks() const;
    bool getSnapToTicks() const;
    
    void setMajorTickSpacing(int value);
    void setMinorTickSpacing(int value);
    void setPaintLabels(bool value);
    void setPaintTicks(bool value);
    void setSnapToTicks(bool value);

    /* Prototypes for the virtual methods */
    virtual std::string getType() const;
    virtual std::string toString() const;

private:
    void create(int min, int max, int value);
    int min;
    int max;
};

/*
 * Class: GTextField
 * -----------------
 * This interactor subclass represents a text field for entering short
 * text strings.  Hitting ENTER in a text field generates a
 * <code>GActionEvent</code> if the text field has a nonempty action command.
 */

class GTextField : public GInteractor {

public:
    enum InputType {INPUT_DOUBLE, INPUT_INTEGER, INPUT_STRING};

    /*
     * Constructor: GTextField
     * Usage: GTextField *field = new GTextField();
     *        GTextField *field = new GTextField(nChars);
     *        GTextField *field = new GTextField(text);
     * --------------------------------------------------
     * Creates a text field capable of holding <code>nChars</code> characters,
     * which defaults to 10.  Assigning an action command to the text field
     * causes it to generate an action event whenever the user types the
     * ENTER key.
     */
    GTextField();
    GTextField(int nChars);
    GTextField(const std::string& text);

    /*
     * Method: getInputType
     * Usage: GTextField::InputType type = field->getInputType();
     * ----------------------------------------------------------
     * Returns the GTextField's expected type of input.
     * The default is INPUT_STRING, which allows any text.
     * If it has been set to INPUT_INTEGER or INPUT_DOUBLE using setInputType,
     * the field will constrain its input to allow only valid numeric data.
     */
    InputType getInputType() const;

    /*
     * Method: getPlaceholder
     * Usage: string str = field->getPlaceholder();
     * --------------------------------------------
     * Returns the light gray placeholder text to appear in the field when no text
     * has been entered.  Initially empty.
     */
    std::string getPlaceholder() const;

    /*
     * Method: getText
     * Usage: string str = field->getText();
     * -------------------------------------
     * Returns the contents of the text field.
     */
    std::string getText() const;

    /* Prototypes for the virtual methods */
    virtual std::string getType() const;

    /*
     * Method: getValueAsDouble
     * Usage: double value = field->getValueAsDouble();
     * ------------------------------------------------
     * Returns the contents of the text field, converted to a double.
     * If the text is unable to be read as a double, throws an ErrorException.
     */
    double getValueAsDouble() const;

    /*
     * Method: getValueAsInteger
     * Usage: int value = field->getValueAsInteger();
     * ----------------------------------------------
     * Returns the contents of the text field, converted to an integer.
     * If the text is unable to be read as an integer, throws an ErrorException.
     */
    int getValueAsInteger() const;

    /*
     * Method: isEditable
     * Usage: if (field->isEditable()) ...
     * -----------------------------------
     * Returns whether the content of the field can be edited by the user.
     * Initially true.
     */
    bool isEditable() const;

    /*
     * Method: setEditable
     * Usage: field->setEditable(false);
     * -----------------------------------
     * Sets whether the content of the field can be edited by the user.
     * Initially true.
     */
    void setEditable(bool value);

    /*
     * Method: setInputType
     * Usage: field->setInputType(GTextField::INPUT_INTEGER);
     * ------------------------------------------------------
     * Sets the GTextField to expect the given type of input.
     * The default is INPUT_STRING, which allows any text.
     * If you specify INPUT_INTEGER or INPUT_DOUBLE, the field will
     * constrain its input to allow only valid numeric data.
     */
    void setInputType(InputType inputType);

    /*
     * Method: setPlaceholder
     * Usage: field->setPlaceholder(str);
     * ----------------------------------
     * Sets a light gray placeholder text to appear in the field when no text
     * has been entered.
     */
    void setPlaceholder(const std::string& text);

    /*
     * Method: setText
     * Usage: field->setText(str);
     * ---------------------------
     * Sets the text of the field to the specified string.
     */
    void setText(const std::string& str);

    /*
     * Method: setValue
     * Usage: field->setValue(value);
     * ------------------------------
     * Sets the text of the field to the specified number converted to a string.
     */
    void setValue(double value);
    void setValue(int value);

    /* Prototypes for the virtual methods */
    virtual std::string toString() const;

    /*
     * Method: valueIsDouble
     * Usage: if (field->valueIsDouble()) ...
     * --------------------------------------
     * Returns true if the text in this field can be treated as a real number.
     */
    bool valueIsDouble() const;

    /*
     * Method: valueIsInteger
     * Usage: if (field->valueIsInteger()) ...
     * ---------------------------------------
     * Returns true if the text in this field can be treated as an integer.
     */
    bool valueIsInteger() const;

    /*
     * Method: valueIsReal
     * Usage: if (field->valueIsReal()) ...
     * ------------------------------------
     * Returns true if the text in this field can be treated as a real number.
     */
    bool valueIsReal() const;

private:
    InputType m_inputType;
    std::string m_placeholder;
};

/*
 * Class: GChooser
 * ---------------
 * This interactor subclass represents a selectable list.  The
 * <code>GChooser</code> constructor creates an empty chooser.
 * Once the chooser has been created, clients can use <code>addItem</code>
 * to add the options.  For example, the following code creates a
 * <code>GChooser</code> containing the four strings
 * <code>"Small"</code>, <code>"Medium"</code>, <code>"Large"</code>,
 * and <code>"X-Large"</code>:
 *
 *<pre>
 *    GChooser *sizeChooser = new GChooser();
 *    sizeChooser->addItem("Small");
 *    sizeChooser->addItem("Medium");
 *    sizeChooser->addItem("Large");
 *    sizeChooser->addItem("X-Large");
 *</pre>
 *<include src="pictures/GInteractorDiagrams/GChooser.html">
 */

class GChooser : public GInteractor {

public:

    /*
     * Constructor: GChooser
     * Usage: GChooser *chooser = new GChooser();
     * ------------------------------------------
     * Creates a chooser that initially contains no items, which are added
     * using the <code>addItem</code> method.  Assigning an action command
     * to the chooser causes it to generate an action event whenever the
     * user selects an item.
     */
    GChooser();

    /*
     * Method: addItem
     * Usage: chooser->addItem(item);
     * ------------------------------
     * Adds a new item consisting of the specified string.
     */
    void addItem(std::string item);

    void addItems(const std::initializer_list<std::string>& items);
    void addItems(const Vector<std::string>& items);

    /*
     * Method: setSelectedItem
     * Usage: chooser->setSelectedItem(item);
     * --------------------------------------
     * Sets the chooser so that it shows the specified item.  If the item
     * does not exist in the chooser, no change occurs.
     */
    void setSelectedItem(std::string item);

    /*
     * Method: getSelectedItem
     * Usage: string item = chooser->getSelectedItem();
     * ------------------------------------------------
     * Returns the current item selected in the chooser.
     */
    std::string getSelectedItem();

    /* Prototypes for the virtual methods */
    virtual std::string getType() const;
    virtual std::string toString() const;
};

#include <private/init.h>   // ensure that Stanford C++ lib is initialized

#endif // _ginteractors_h
