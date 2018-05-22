// <snippet7>
// <snippet17>
#pragma once
// </snippet17>

class file_reader : public concurrency::agent
{
public:
   // <snippet4>
   explicit file_reader(const std::string& file_name, 
      concurrency::ITarget<std::string>& target)
      : _file_name(file_name)
      , _target(target)
   {
   }

   explicit file_reader(const std::string& file_name, 
      concurrency::ITarget<std::string>& target,
      concurrency::Scheduler& scheduler)
      : agent(scheduler)
      , _file_name(file_name)
      , _target(target)
   {
   }

   explicit file_reader(const std::string& file_name, 
      concurrency::ITarget<std::string>& target,
      concurrency::ScheduleGroup& group)
      : agent(group) 
      , _file_name(file_name)
      , _target(target)
   {
   }
   // </snippet4>
   
   // Retrieves any error that occurs during the life of the agent.
   // <snippet5>
   bool get_error(std::exception& e)
   {
      return try_receive(_error, e);
   }
   // </snippet5>
   
protected:
   // <snippet6>
   void run()
   {
      FILE* stream;
      try
      {
         // Open the file.
         if (fopen_s(&stream, _file_name.c_str(), "r") != 0)
         {
            // Throw an exception if an error occurs.            
            throw std::exception("Failed to open input file.");
         }
      
         // Create a buffer to hold file data.
         char buf[1024];

         // Set the buffer size.
         setvbuf(stream, buf, _IOFBF, sizeof buf);
         
         // Read the contents of the file and send the contents
         // to the target.
         while (fgets(buf, sizeof buf, stream))
         {
            asend(_target, std::string(buf));
         }   
         
         // Send the empty string to the target to indicate the end of processing.
         asend(_target, std::string(""));

         // Close the file.
         fclose(stream);
      }
      catch (const std::exception& e)
      {
         // Send the empty string to the target to indicate the end of processing.
         asend(_target, std::string(""));

         // Write the exception to the error buffer.
         send(_error, e);
      }

      // Set the status of the agent to agent_done.
      done();
   }
   // </snippet6>

private:
   // <snippet3>
   std::string _file_name;
   concurrency::ITarget<std::string>& _target;
   concurrency::overwrite_buffer<std::exception> _error;
   // </snippet3>
};
// </snippet7>

/*
// <snippet2>
class file_reader : public concurrency::agent
{
public:
protected:
private:
};
// </snippet2>
*/