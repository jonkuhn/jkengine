#pragma once

#include <cassert>

namespace JkEng
{
    // A non-owning pointer that will be populated later (and exactly once).
    //
    // This is used in the scene definition pattern where pointers
    // to AfterBuildPtr<T> are passed to definition objects and when
    // the scene is actually constructed form the definition the 
    // AfterBuildPtr<T> objects will be initialized with a pointer
    // to the object constructed from the definition.
    //
    // Code holding an AfterBuildPtr does not own the pointer and must not
    // attempt to delete it for free it.
    //
    // For example, a Graphics::SpriteDefinition takes an AfterBuildPtr<Graphics::ISprite>*
    // as a constructor argument.  The AfterBuildPtr<Graphics::ISprite> will be initialized after
    // the Graphics::SceneDefinition containing the Graphics::SpriteDefinition
    // is used to construct a Graphics::IScene by passing the definition to
    // Graphics::IEngine::CreateScene().  After the call to CreateScene returns successfully,
    // the JkEng::AfterBuildPtr<ISprite> is guaranteed to be initialized to point to an actual
    // ISprite implementation.  After that the AfterBuildPtr<ISprite> cannot be re-assigned to point to
    // a different object.
    template<typename T>
    class AfterBuildPtr
    {
    public:
        inline void Initialize(T* ptr)
        {
            assert(_ptr == nullptr && "AfterBuildPtr: Initialize must only be called once");
            _ptr = ptr;
        }

        inline T* Get() const
        {
            assert(_ptr != nullptr && "AfterBuildPtr: Get called before Initialize");
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
