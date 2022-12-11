class cStrategy
{
public:

    void clear();

    void combine();
    
    void read(const std::string &fname);

    void name( const std::string& name )
    {
        myName = name;
    }

    std::string name() const
    {
        return myName;
    }

    std::string text() const;
    std::string textSummary() const;

private:
    struct sResult
    {
        std::string sdate;
        int ymd;
        float profit;
    };
    std::string myName;
    std::vector<sResult> myResult;
    float myProfit;
    float myMaxLoss;
    float myRank;
    static std::vector<cStrategy> theStrategy;

    void parse(const std::string &line);

    void add(
        const std::string &date,
        float value);

    float rank() const
    {
        return myRank;
    }

    void rankCalc();
    void rankOrder();

    /// @brief combine strategies
    /// @param vs vector of strategis to combine
    void combine(
        std::vector< cStrategy > vs    );
    
    /// @brief combine this strategy with another
    /// @param other 
    void combine( const cStrategy& other );
};
