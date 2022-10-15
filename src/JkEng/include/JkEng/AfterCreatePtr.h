#pragma once

#include <cassert>

namespace JkEng
{
    // A non-owning pointer that will be populated later (and exactly once).
    //
    // This is used in the scene definition pattern where pointers
    // to AfterCreatePtr<T> are passed to definition objects and when
    // the scene is actually constructed form the definition the 
    // AfterCreatePtr<T> objects will be initialized with a pointer
    // to the object constructed from the definition.
    //
    // Code holding an AfterCreatePtr does not own the pointer and must not
    // attempt to delete it for free it.
    //
    // For example, a Graphics::SpriteDefinition takes an AfterCreatePtr<Graphics::ISprite>*
    // as a constructor argument.  The AfterCreatePtr<Graphics::ISprite> will be initialized after
    // the Graphics::SceneDefinition containing the Graphics::SpriteDefinition
    // is used to construct a Graphics::IScene by passing the definition to
    // Graphics::IEngine::CreateScene().  After the call to CreateScene returns successfully,
    // the JkEng::AfterCreatePtr<ISprite> is guaranteed to be initialized to point to an actual
    // ISprite implementation.  After that the AfterCreatePtr<ISprite> cannot be re-assigned to point to
    // a different object.
    template<typename T>
    class AfterCreatePtr
    {
    public:
        inline void Initialize(T* ptr)
        {
            assert(_ptr == nullptr && "AfterCreatePtr: Initialize must only be called once");
            _ptr = ptr;
        }

        inline T* Get() const
        {
            assert(_ptr != nullptr && "AfterCreatePtr: Get called before Initialize");
            return _ptr;
        }

        inline T& operator*() const
        {
            return *(Get());
        }

        inline T* operator->() const
        {
            return Get();
        }

    private:
        T* _ptr = nullptr;
    };
}
