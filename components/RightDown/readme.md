#### 数据格式

```
    var freqData = [
        { State: '0 - 50', Count: 100, freq: { Domain: 4786, IP: 1319, Cert: 249, ASN: 100 }, area: { Top: 1, Mid: 9, Bot: 11 } }
        , { State: '50 - 100', Count: 22, freq: { Domain: 1101, IP: 412, Cert: 674, ASN: 23 }, area: { Top: 2, Mid: 8, Bot: 12 } }
        , { State: '100 - 150', Count: 31, freq: { Domain: 932, IP: 2149, Cert: 418, ASN: 34 }, area: { Top: 3, Mid: 7, Bot: 13 } }
        , { State: '150 - 200', Count: 50, freq: { Domain: 832, IP: 1152, Cert: 1862, ASN: 45 }, area: { Top: 4, Mid: 6, Bot: 14 } }
        , { State: '200 - 250', Count: 300, freq: { Domain: 4481, IP: 3304, Cert: 948, ASN: 56 }, area: { Top: 5, Mid: 5, Bot: 15 } }
        , { State: '250 - 300', Count: 64, freq: { Domain: 324, IP: 3434, Cert: 436, ASN: 67 }, area: { Top: 6, Mid: 4, Bot: 16 } }
        , { State: '300 - 350', Count: 97, freq: { Domain: 4566, IP: 546, Cert: 675, ASN: 78 }, area: { Top: 7, Mid: 3, Bot: 17 } }
        , { State: '350 - 400', Count: 18, freq: { Domain: 8676, IP: 876, Cert: 665, ASN: 89 }, area: { Top: 8, Mid: 2, Bot: 18 } }
        , { State: '> 400', Count: 9, freq: { Domain: 3421, IP: 987, Cert: 354, ASN: 90 }, area: { Top: 9, Mid: 1, Bot: 19 } }
    ];

    var type2field = {
        'Domain': { 'Top': 12, 'Mid': 90, 'Bot': 21 }
        , 'IP': { 'Top': 22, 'Mid': 80, 'Bot': 41 }
        , 'Cert': { 'Top': 32, 'Mid': 20, 'Bot': 30 }
        , 'ASN': { 'Top': 2, 'Mid': 60, 'Bot': 50 }
    }

    var field2type = {
        'Top': { 'Domain': 12, 'IP': 90, 'Cert': 21, 'ASN': 10}
        , 'Mid': { 'Domain': 22, 'IP': 80, 'Cert': 41, 'ASN': 30}
        , 'Bot': { 'Domain': 32, 'IP': 20, 'Cert': 30, 'ASN': 40}
    }
```

#### 实现效果

http://182.61.150.155:2000/
