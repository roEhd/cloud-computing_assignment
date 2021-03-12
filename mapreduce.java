package wordcount;
import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.Map;
import java.util.Map.Entry;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapred.KeyValueTextInputFormat;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;


public class WordCount {
public static Map <String, Integer> map = new HashMap<String, Integer>();

public static class TokenizerMapper extends Mapper<Object, Text, Text, IntWritable>{
private final static IntWritable one = new IntWritable(1);
private Text word = new Text();
public void map(Object key, Text value, Context context) throws IOException, InterruptedException{
StringTokenizer itr = new StringTokenizer(value.toString());
while(itr.hasMoreTokens()) {
word.set(itr.nextToken());
context.write(word, one);
}
}
}

public static class IntSumReducer extends Reducer<Text,IntWritable,Text,IntWritable>{
Text forkey = new Text();
IntWritable forvalue = new IntWritable();
public void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException{
int sum = 0;
for(IntWritable val : values) {
sum += val.get();
}
forvalue.set(sum);
context.write(key, forvalue);
}
}

public static class SecondMapper extends Mapper<Object, Text, Text, IntWritable>{
private IntWritable one = new IntWritable();
private Text word = new Text();
public void map(Object key, Text value, Context context) throws IOException, InterruptedException{
StringTokenizer itr = new StringTokenizer(value.toString());
word.set(itr.nextToken());
one.set(Integer.parseInt(itr.nextToken()));
context.write(word, one);
map.put(word.toString(),one.get());
}
}

public static class SecondReducer extends Reducer<Text,IntWritable,Text,IntWritable>{
Text forkey = new Text();
IntWritable forvalue = new IntWritable();


public void cleanup(Context context) throws IOException, InterruptedException{
//quick sort in descending order
List<Entry<String,Integer>> list_entries = new ArrayList<Entry<String,Integer>>(map.entrySet());
Collections.sort(list_entries, new Comparator <Entry<String, Integer>>(){
public int compare(Entry<String, Integer> obj1, Entry<String, Integer> obj2) {
return obj2.getValue().compareTo(obj1.getValue());
}
});
System.out.println("Sorted Result");
for(Entry<String,Integer> entry :  list_entries) {
forkey.set("**"+entry.getKey());
forvalue.set(entry.getValue());
System.out.println(forkey+" "+forvalue);
context.write(forkey, forvalue);
}
}
}

public static void main(String[] args) throws Exception{
Configuration conf = new Configuration();
Job job = Job.getInstance(conf, "word count");

job.setJarByClass(WordCount.class);
job.setMapperClass(TokenizerMapper.class);
job.setReducerClass(IntSumReducer.class);
job.setNumReduceTasks(1);
job.setOutputKeyClass(Text.class);
job.setOutputValueClass(IntWritable.class);

FileInputFormat.addInputPath(job, new Path(args[0]));
FileOutputFormat.setOutputPath(job, new Path(args[1]));
job.waitForCompletion(true);

Configuration conf2 = new Configuration();
conf2.set("key.value.separator.in.input.line", ",");
Job job2 = Job.getInstance(conf2, "word count 2");

job2.setJarByClass(WordCount.class);
job2.setMapperClass(SecondMapper.class);
job2.setReducerClass(SecondReducer.class);

job2.setOutputKeyClass(Text.class);
job2.setOutputValueClass(IntWritable.class);

FileInputFormat.addInputPath(job2, new Path(args[1]));
FileOutputFormat.setOutputPath(job2, new Path(args[2]));
job2.waitForCompletion(true);
}
}
