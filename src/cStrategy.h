class cStrategy
{
public:

    void clear();

    /**
     * @brief Combine strategies
     * 
     * Combine the strategy in the last position
     * ( assumed to be a new original single strategy )
     * with each of the other strategies, original and combined
     */
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
    static std::vector<cStrategy> theStrategyRankOrder;

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
  
    /// @brief combine this strategy with another
    /// @param other 
    void combine( const cStrategy& other );
};
