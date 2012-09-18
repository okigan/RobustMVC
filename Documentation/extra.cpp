/// Some details
class CView : public CWnd
{
protected:
    /*! Some details about the Test class
    */
    CDocument* m_pDocument;
};

/// Some details
class CObject {};


/// Some details
class CCmdTarget : public CObject {};

/// Some details
class CDocument : public CCmdTarget {};

/// Some details
class CWnd : public CCmdTarget {};

/// Some details
class CDialogEx : public CDialog {};

/// Some details
class CDialog : public CWnd {};

namespace std {

    template <typename T>
    class unique_ptr {	
    protected:
        /// ptr
        T* ptr; 
    };

}
