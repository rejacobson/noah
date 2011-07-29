/**
 * Reference counting pointer.
 *
 * Inspired by: http://archive.gamedev.net/reference/articles/article1060.asp#overhead
 */
#ifndef _SAFE_PTR_H
#define _SAFE_PTR_H

namespace noah
{

template <class T>
struct safe_ptr_reference_counter
{
  safe_ptr_reference_counter( T *ptr = 0 ) : ptr_(ptr), ref_count_(0) { }
  ~safe_ptr_reference_counter() { clear(); }

  void clear( void )
  {
    if ( ptr_ )
    {
      delete ptr_;
      ptr_ = 0;
    }
  }

  // Add 1 to the reference count.
  void addRef() { ref_count_++; }

  // Subtract 1 from the reference count.
  // Returns true if the reference count has reached 0
  // and the object should be deleted.
  bool subRef() { return (--ref_count_ <= 0); }

  // Number of safe_ptr references to this counter
  int ref_count_;

  // Pointer to the actual object being used.
  T *ptr_;
};


template <class T>
class safe_ptr
{
  public:
    // Construct from normal pointer, default to NULL
    safe_ptr( T *ptr = 0 )
    {
      ref_counter_ = new safe_ptr_reference_counter<T>( ptr );
      addRef();
    }

    // Construct from another smart pointer.  Copy Constructor
    safe_ptr( const safe_ptr<T> &p )
    {
      ref_counter_ = p.ref_counter_;
      addRef();
    }

    // Destructor.
    ~safe_ptr() { subRef(); }

    // Assignment operator.
    safe_ptr &operator= ( T *ptr )
    {
      if ( ref_counter_->ptr_ != ptr )
      {
        subRef();
        ref_counter_ = new safe_ptr_reference_counter<T>( ptr );
        addRef();
      }

      return *this;
    }

    // Assignment operator.
    safe_ptr &operator= ( const safe_ptr &p )
    {
      subRef();
      ref_counter_ = p.ref_counter_;
      addRef();
      return *this;
    }

    // Dereferencing operator. Provided to behave like the normal pointer.
    T& operator* () const { return *( ref_counter_->ptr_ ); }

    // Member access operator. Provided to behave like the normal pointer.
    T* operator-> () const { return ref_counter_->ptr_; }

    // Conversion operators
    operator T* () const { return ref_counter_->ptr_; }
    operator const T* () const { return ref_counter_->ptr_; }

    // boolean test for NULL
	  operator bool () const { return ref_counter_->ptr_ != 0; }

    void clear( void )
    {
      if ( ref_counter_ && ref_counter_->ptr_ )
        ref_counter_->clear();
    }

  private:
    void addRef()
    {
      // Only change if non-null
      if (ref_counter_) ref_counter_->addRef();
    }

    void subRef()
    {
      // Only change if non-null
      if (ref_counter_)
      {
        // Subtract and test if this was the last pointer.
	      if (ref_counter_->subRef())
        {

          delete ref_counter_;
          ref_counter_ = 0;
        }
      }
    }

    safe_ptr_reference_counter<T> *ref_counter_;
};

} // namespace noah

#endif