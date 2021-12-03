/*
 * MIT License
 *
 * Copyright (c) 2020-2021 FrozenStorm Interactive
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "CoreMinimal.h"
#include "SignalRValue.h"



class SIGNALR_API IHubConnection : public TSharedFromThis<IHubConnection>
{
public:
    DECLARE_DELEGATE_OneParam(FOnMethodInvocation, const TArray<FSignalRValue>&)
    DECLARE_DELEGATE_OneParam(FOnMethodCompletion, const FSignalRValue&)

    virtual FOnMethodInvocation& On(FName EventName) = 0;

    virtual FOnMethodCompletion& Invoke(FName EventName, const TArray<FSignalRValue>& InArguments = TArray<FSignalRValue>()) = 0;

    template <typename... ArgTypes>
    FORCEINLINE FOnMethodCompletion& Invoke(FName EventName, ArgTypes... Arguments)
    {
        static_assert(TAnd<TIsConstructible<FSignalRValue, ArgTypes>...>::Value, "Invalid argument type passed to IHubConnection::Invoke");
        return Invoke(EventName, TArray<FSignalRValue> { MoveTemp(Arguments)... });
    }

    virtual void Send(FName EventName, const TArray<FSignalRValue>& InArguments = TArray<FSignalRValue>()) = 0;

    template <typename... ArgTypes>
    FORCEINLINE void Send(FName EventName, ArgTypes... Arguments)
    {
        static_assert(TAnd<TIsConstructible<FSignalRValue, ArgTypes>...>::Value, "Invalid argument type passed to IHubConnection::Send");
        Send(EventName, TArray<FSignalRValue> { MoveTemp(Arguments)... });
    }

protected:

    /**
     * Destructor
     */
    virtual ~IHubConnection();
};

typedef TSharedPtr<IHubConnection> IHubConnectionPtr;
