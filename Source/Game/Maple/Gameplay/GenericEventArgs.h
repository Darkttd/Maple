#pragma once

#include <Maple/FWD.h>
#include <Bibim/UIEventID.h>
#include <Bibim/UIEventArgs.h>

namespace Maple
{
    struct NullEventVariable
    {
        operator const char* () const { return nullptr; }
    };

    template <typename T1 = NullEventVariable,
              typename T2 = NullEventVariable,
              typename T3 = NullEventVariable,
              typename T4 = NullEventVariable,
              typename T5 = NullEventVariable,
              typename T6 = NullEventVariable,
              typename T7 = NullEventVariable,
              typename T8 = NullEventVariable,
              typename T9 = NullEventVariable>
    class GenericEventArgs : public Bibim::UIEventArgs
    {
        public:
            GenericEventArgs() { }
            GenericEventArgs(Bibim::UIVisual* target,
                                  const T1& v1 = T1(),
                                  const T2& v2 = T2(),
                                  const T3& v3 = T3(),
                                  const T4& v4 = T4(),
                                  const T5& v5 = T5(),
                                  const T6& v6 = T6(),
                                  const T7& v7 = T7(),
                                  const T8& v8 = T8(),
                                  const T9& v9 = T9())
                : Bibim::UIEventArgs(target),
                  v1(v1), v2(v2), v3(v3), v4(v4), v5(v5), v6(v6), v7(v7), v8(v8), v9(v9)
            {
            }

            GenericEventArgs(const GenericEventArgs& o)
                : Bibim::UIEventArgs(o),
                  v1(o.v1), v2(o.v2), v3(o.v3), v4(o.v4), v5(o.v5), v6(o.v6), v7(o.v7), v8(o.v8), v9(o.v9)
            {
            }

            virtual ~GenericEventArgs() { }

            virtual Bibim::UIEventArgs* Clone() const { return new GenericEventArgs<T1, T2, T3, T4, T5, T6, T7, T8, T9>(*this); }
            virtual void Serialize(Serializer& context) const
            {
                UIEventArgs::Serialize(context);
                context.Push(v1);
                context.Push(v2);
                context.Push(v3);
                context.Push(v4);
                context.Push(v5);
                context.Push(v6);
                context.Push(v7);
                context.Push(v8);
                context.Push(v9);
            }

        private:
            T1 v1; T2 v2; T3 v3; T4 v4; T5 v5; T6 v6; T7 v7; T8 v8; T9 v9;
    };

    inline Bibim::UIEventArgs Args(Bibim::UIVisual* target)
    {
        return Bibim::UIEventArgs(target);
    }

    template <typename T1>
    inline GenericEventArgs<T1> Args(Bibim::UIVisual* target,
                                     const T1& v1)
    {
        return GenericEventArgs<T1>(target, v1);
    }

    template <typename T1, typename T2>
    inline GenericEventArgs<T1, T2> Args(Bibim::UIVisual* target,
                                         const T1& v1, const T2& v2)
    {
        return GenericEventArgs<T1, T2>(target, v1, v2);
    }

    template <typename T1, typename T2, typename T3>
    inline GenericEventArgs<T1, T2, T3> Args(Bibim::UIVisual* target,
                                             const T1& v1, const T2& v2, const T3& v3)
    {
        return GenericEventArgs<T1, T2, T3>(target, v1, v2, v3);
    }

    template <typename T1, typename T2, typename T3, typename T4>
    inline GenericEventArgs<T1, T2, T3, T4> Args(Bibim::UIVisual* target,
                                                 const T1& v1, const T2& v2, const T3& v3,
                                                 const T4& v4)
    {
        return GenericEventArgs<T1, T2, T3, T4>(target, v1, v2, v3, v4);
    }

    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    inline GenericEventArgs<T1, T2, T3, T4, T5> Args(Bibim::UIVisual* target,
                                                     const T1& v1, const T2& v2, const T3& v3,
                                                     const T4& v4, const T5& v5)
    {
        return GenericEventArgs<T1, T2, T3, T4, T5>(target, v1, v2, v3, v4, v5);
    }

    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    inline GenericEventArgs<T1, T2, T3, T4, T5, T6> Args(Bibim::UIVisual* target,
                                                         const T1& v1, const T2& v2, const T3& v3,
                                                         const T4& v4, const T5& v5, const T6& v6)
    {
        return GenericEventArgs<T1, T2, T3, T4, T5, T6>(target, v1, v2, v3, v4, v5, v6);
    }

    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    inline GenericEventArgs<T1, T2, T3, T4, T5, T6, T7> Args(Bibim::UIVisual* target,
                                                             const T1& v1, const T2& v2, const T3& v3,
                                                             const T4& v4, const T5& v5, const T6& v6,
                                                             const T7& v7)
    {
        return GenericEventArgs<T1, T2, T3, T4, T5, T6, T7>(target, v1, v2, v3, v4, v5, v6, v7);
    }

    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    inline GenericEventArgs<T1, T2, T3, T4, T5, T6, T7, T8> Args(Bibim::UIVisual* target,
                                                                 const T1& v1, const T2& v2, const T3& v3,
                                                                 const T4& v4, const T5& v5, const T6& v6,
                                                                 const T7& v7, const T8& v8)
    {
        return GenericEventArgs<T1, T2, T3, T4, T5, T6, T7, T8>(target, v1, v2, v3, v4, v5, v6, v7, v8);
    }

    template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    inline GenericEventArgs<T1, T2, T3, T4, T5, T6, T7, T8, T9> Args(Bibim::UIVisual* target,
                                                                     const T1& v1, const T2& v2, const T3& v3,
                                                                     const T4& v4, const T5& v5, const T6& v6,
                                                                     const T7& v7, const T8& v8, const T9& v9)
    {
        return GenericEventArgs<T1, T2, T3, T4, T5, T6, T7, T8, T9>(target, v1, v2, v3, v4, v5, v6, v7, v8, v9);
    }
}