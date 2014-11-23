package model;

/** 
* BD: Hamming Toolkit 
* Reference: 
*      - Hamming distance 
*        http://en.wikipedia.org/wiki/Hamming_distance 
*      - Levenshtein distance 
*        http://en.wikipedia.org/wiki/Levenshtein_distance 
*      - Java example 9 - Hamming Distance 
*        http://blog.csdn.net/kindterry/article/details/6581344 
*    
* @author John 
* http://puremonkey2010.blogspot.tw/2012/11/java-hamming-distance.html
* 
*/  

public class HammingUtil {  
    /** 
     * BD: Calculate Hamming Distance between str1 and str2. 
     * @param str1: Input string1 
     * @param str2: Input string2 
     * @return Hamming distance 
     */  
    public static int getDistance(String str1, String str2)  
    {  
          
        if(str1.length()==str2.length())  
        {  
            int distance=0;  
            for  ( int  i =  0 ; i < str1.length(); i++) {    
                if  (str1.charAt(i) != str2.charAt(i)) {    
                    distance++;    
                }    
            }   
            return distance;  
        }  
        else if(str1.length()>str2.length())  
        {  
            if(str1.indexOf(str2)>=0) return str1.length()-str2.length();  
            else  
            {  
                return LevenshteinDistance(str1, str2);  
            }  
        }  
        else  
        {  
            if(str2.indexOf(str1)==0) return str2.length()-str1.length();  
            else  
            {  
                return LevenshteinDistance(str1, str2);  
            }  
        }  
    }  
      
    public static int LevenshteinDistance(String s, String t)  
    {  
        if(s.length()==0) return t.length();  
        else if(t.length()==0) return s.length();  
          
        int cost = 0;  
        if(s.charAt(0) != t.charAt(0)) cost = 1;          
        return Math.min(LevenshteinDistance(s.substring(1), t)+1,   
                        Math.min(LevenshteinDistance(s, t.substring(1))+1,   
                                LevenshteinDistance(s.substring(1), t.substring(1))+cost));  
        //return LevenshteinDistance(s.substring(1), t.substring(1))+cost;  
    }  
}  