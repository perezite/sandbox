using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace extractvids
{
    class Program
    {
        static void Main(string[] args)
        {
            var text = File.ReadAllText("vids.txt");

            var positions = FindAllIndexesOf(text, "/watch").ToList();
            var urls = positions.Select(x => ExtractUntil(text, x, '\"')).ToList();
            urls = urls.Where(x => x.Contains("index")).ToList();
            urls = urls.Distinct().ToList();
            urls = urls.Select(x => $"https://www.youtube.com/{x}").ToList();

            File.WriteAllLines("urls.txt", urls.ToArray());
        }

        // https://stackoverflow.com/questions/6865419/indexof-for-multiple-results
        private static IEnumerable<int> FindAllIndexesOf(string str, string searchstring)
        {
            int minIndex = str.IndexOf(searchstring);
            while (minIndex != -1)
            {
                yield return minIndex;
                minIndex = str.IndexOf(searchstring, minIndex + searchstring.Length);
            }
        }

        private static string ExtractUntil(string str, int beginPos, char endCharacter)
        {
            var endPos = str.IndexOf(endCharacter, beginPos);
            var extracted = str.Substring(beginPos, endPos - beginPos);
            return extracted;
        }
    }
}
