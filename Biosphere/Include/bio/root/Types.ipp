namespace bio
{
    template<class Wrapped>
    ResultWrap<Wrapped>::ResultWrap(Result Res, Wrapped Object)
    {
        this->res = Res;
        this->obj = Object;
    }

    template<class Wrapped>
    ResultWrap<Wrapped>::operator Result()
    {
        return this->res;
    }

    template<class Wrapped>
    Wrapped ResultWrap<Wrapped>::AssertOk()
    {
        this->res.AssertOk();
        return this->obj;
    }
}