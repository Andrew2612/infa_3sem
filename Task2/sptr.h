template <class T>
class UniquePtr {
public:
    UniquePtr() noexcept : _ptr() { }
    UniquePtr(T* p) noexcept : _ptr(p) { }

    UniquePtr(const UniquePtr& o) = delete;
    UniquePtr& operator=(const UniquePtr& o) = delete;
    
    UniquePtr(UniquePtr&&) = default;

    UniquePtr& operator=(UniquePtr&& o) noexcept 
    {
        Reset(o.Release());
        return *this;
    }

    ~UniquePtr() noexcept
    {
        Delete(_ptr);
    }


    T* operator->() {return _ptr;}

    T& operator*() {return *_ptr;}
    
private:
    T* _ptr;

    T* Release() noexcept
    {
        T* p = _ptr;
        _ptr = nullptr;
        return p;
    }

    void Reset(T* p) noexcept
    {
        T* pp = _ptr;
        _ptr = p;
        if (pp != _ptr)
        {
            Delete(pp);
        }
    }

    void Delete(T* pp) const noexcept
    {
        if (pp != nullptr)
        {
            delete pp;
        }
    }
};


// ================================================
struct RefCntBlock
{
    size_t strong, weak;
};

template <class T>
class WeakPtr;

template <class T>
class SharedPtr {   
    friend class WeakPtr<T>;
public:
    SharedPtr() noexcept : _ptr(), _block() { }

    SharedPtr(const SharedPtr& o) noexcept : _ptr(o.GetPtr()), _block(o.GetBlock())
    {
        Inc();
    }

    SharedPtr& operator=(const SharedPtr& o) noexcept
    {
        if (_block != o.GetBlock())
        {
            this->~SharedPtr();
        }

        _ptr = o.GetPtr();
        _block = o.GetBlock();
        Inc();
        return *this;
    }
    
    SharedPtr(SharedPtr&& o) noexcept : _ptr(o.GetPtr()), _block(o.GetBlock())
    {
        Inc();
        o.Reset();
    };
    
    SharedPtr& operator=(SharedPtr&& o) noexcept
    {
        if (_block != o.GetBlock())
        {
            this->~SharedPtr();
        }

        _ptr = o.GetPtr();
        _block = o.GetBlock();
        Inc();
        o.Reset();
        return *this;
    }
    
    SharedPtr(T* p) noexcept : _ptr(p), _block(new RefCntBlock())
    {
        _block->strong++;
    }
    
    // Implementation below
    SharedPtr(const WeakPtr<T>& o) noexcept ;

    ~SharedPtr()
    {
        if (_block != nullptr)
        {
            _block->strong--;
            if (_block->strong == 0)
            {
                delete _ptr;

                if (_block->weak == 0)
                {
                    delete _block;
                }
            }
        }
    }

    
    T* operator->() {return _ptr;}

    T& operator*() {return *_ptr;}
    
    
    void Reset() noexcept
    {
        this->~SharedPtr();

        _ptr = nullptr;
        _block = nullptr;
    }

private:
    T* _ptr;
    RefCntBlock* _block;


    RefCntBlock* GetBlock() const noexcept
    {
        return _block;
    }
    T* GetPtr() const noexcept
    {
        return _ptr;
    }

    void Inc() noexcept
    {
        if (_block != nullptr)
        {
            _block->strong++;
        }
    }
};

template <class T>
class WeakPtr {
    friend class SharedPtr<T>;
public:
    WeakPtr() noexcept : _block() { }

    WeakPtr(const WeakPtr& o) noexcept : _ptr(o.GetPtr()), _block(o.GetBlock())
    {
        Inc();
    }

    WeakPtr& operator=(const WeakPtr& o) noexcept
    {
        if (_block != o.GetBlock())
        {
            this->~WeakPtr();
        }

        _ptr = o.GetPtr(),
        _block = o.GetBlock();
        Inc();
        return *this;
    }
    
    WeakPtr(WeakPtr&& o) noexcept : _ptr(o.GetPtr()), _block(o.GetBlock())
    {
        Inc();
        o.Reset();
    };
    
    WeakPtr& operator=(WeakPtr&& o) noexcept
    {
        if (_block != o.GetBlock())
        {
            this->~WeakPtr();
        }

        _block = o.GetBlock();
        _ptr = o.GetPtr(),
        Inc();
        o.Reset();
        return *this;
    }
    
    WeakPtr(const SharedPtr<T>& o) noexcept : _ptr(o.GetPtr()), _block(o.GetBlock())
    {
        Inc();
    };
    
    WeakPtr& operator=(const SharedPtr<T>& o) noexcept
    {
        if (_block != o.GetBlock())
        {
            this->~WeakPtr();
        }

        _block = o.GetBlock();
        _ptr = o.GetPtr();
        Inc();
        return *this;
    }

    ~WeakPtr () noexcept
    {
        if (_block != nullptr)
        {
            _block->weak--;
            if (_block->strong == 0 & _block->weak == 0)
            {
                delete _block;
            }
        }
    }
    
    // Replaces pointer with nullptr
    void Reset() noexcept
    {
        this->~WeakPtr();

        _block = nullptr;
        _ptr = nullptr;
    }
    
    bool Expired() const noexcept 
    {
        return (_block == nullptr || _block->strong == 0);
    }
    
    SharedPtr<T> Lock() noexcept
    {
        return SharedPtr<T>(*this);
    }
    

private:
    T* _ptr;
    RefCntBlock* _block;

    RefCntBlock* GetBlock() const noexcept
    {
        return _block;
    }
    T* GetPtr() const noexcept
    {
        return _ptr;
    }

    void Inc() noexcept
    {
        if (_block != nullptr)
        {
            _block->weak++;
        }
    }
};

template <class T>
SharedPtr<T>::SharedPtr(const WeakPtr<T>& o) noexcept : _ptr(o.GetPtr()), _block(o.GetBlock())
{
    Inc();
}
