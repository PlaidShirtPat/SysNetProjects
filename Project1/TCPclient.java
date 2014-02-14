/*
 * TCPclient.java
 * Systems and Networks II
 * Project 1
 * Patrick Brown
 *
 * This file describes the functions to be implemented by the TCPclient class
 * You may also implement any auxillary functions you deem necessary.
 */
import java.io.*;
import java.net.*;


public class TCPclient
{
	private Socket _socket; // the socket for communication with a server
	
	/**
	 * Constructs a TCPclient object.
	 */
	public TCPclient()
	{
	}
	
	/**
	 * Creates a streaming socket and connects to a server.
	 *
	 * @param host - the ip or hostname of the server
	 * @param port - the port number of the server
	 *
	 * @return - 0 or a negative number describing an error code if the connection could not be established
   * -1 - UnkownHostException
   * -2 - IOException
   * -3 - SecurityException
   * -4 - IllegalArgumentException
	 */
	public int createSocket(String host, int port) throws Exception
	{

    try
    { _socket = new Socket(host, port, InetAddress.getLocalHost(), 0);}
    catch( UnknownHostException e)
    { return -1;}
    catch( IOException e)
    { 
      System.out.println(e.toString());
      return -2;
    }
    catch( SecurityException e)
    {
      System.out.println(e.toString());
      return -3;
    }
    catch( IllegalArgumentException e)
    { 
      System.out.println(e.toString());
      return -4;
    }

    System.out.println(_socket.getLocalPort());
    return 0;

	}

	/**
	 * Sends a request for service to the server. Do not wait for a reply in this function. This will be
	 * an asynchronous call to the server.
	 * 
	 * @request - the request to be sent
	 *
	 * @return - 0, if no error; otherwise, a negative number indicating the error
   * -1, the _socket attribute is null
   * -2, could not create an output stream
   * -3, could not send message
	 */

	public int sendRequest(String request)
	{

    if(_socket == null)
      return -1;

    PrintStream outBuffer;

    try
    {outBuffer = new PrintStream(_socket.getOutputStream(), true, "US-ASCII");}
    catch(Exception e)
    {
      System.out.println(e.toString());
      return -2;
    }
    
    try
    {outBuffer.print(request);}
    catch(Exception e)
    {
      System.out.println(e.toString());
      return -3;
    }


    return 0;
	}
	
	/**
	 * Receives the server's response. Also displays the response in a
	 * clear and concise format.
	 *
	 * @return - the server's response or NULL if an error occured
	 */
	public String receiveResponse()
	{
		
    if(_socket == null)
      return null;

    BufferedReader inBuffer;

    String result = "";

    try
    {inBuffer = new BufferedReader(new InputStreamReader(_socket.getInputStream()));}
    catch(Exception e)
    {
      System.out.println(e.toString());
      return null;
    }

    try
    {result = inBuffer.readLine();}
    catch(Exception e)
    {
      System.out.println(e.toString());
      return null;
    }

    //validate our result, if invalid, return null
    String formattedResult = checkAndFormatReply(result);
    if(formattedResult == null)
      return null;

    return result;
	}


	/*
	 * Validates and formats a reply
	 *
   * @return - the formated reply if valid
   * null, if not valid
   * 
	 */
  private static String checkAndFormatReply(String message)
  {
    //check for <reply></reply>
    String firstTag = message.substring(0, 7);
    String lastTag = message.substring(message.length()-8, message.length());
    if( firstTag.equals("<reply>"))
      if(lastTag.equals("</reply>"))
        //get the message without the tags
        return message.substring(7, message.length()-8) ; 

    firstTag = message.substring(0, 14);
    lastTag = message.substring(message.length()-15, message.length());
    if( firstTag.equals("<replyLoadAvg>"))
      if(lastTag.equals("</replyLoadAvg>"))
      {
        //get rid of the tags
        message = message.substring(14, message.length()-15) ; 

        int firstColon = message.indexOf(":");
        int lastColon = message.lastIndexOf(":");
        
        //make sure there are two colons
        if(firstColon == lastColon)
          return null;

        //ensure that both colons exist
        if(firstColon == 0 || lastColon == 0)
          return null;

        //get the individual load averages using the indexes of the ":"s
        String formattedMessage = "Load over last minute: " 
          +  message.substring(0, firstColon-1) + "\n";
        formattedMessage = formattedMessage + "Load over 5 minutes: " 
          + message.substring(firstColon+1, lastColon)  + "\n";
        formattedMessage = formattedMessage + "Load over 10 minutes: " 
          + message.substring(lastColon+1, message.length())  + "\n";

        return formattedMessage;
      }

    firstTag = message.substring(0, 7);
    lastTag = message.substring(message.length()-8, message.length());
    if( firstTag.equals("<error>"))
      if(lastTag.equals("</error>"))
        return message.substring(7, message.length()-8) ; 

    return null;
  }
	
	/*
    * Prints the response to the screen in a formatted way.
    *
    * response - the server's response as an XML formatted string
    *
    */
	public static void printResponse(String response)
	{
    response = checkAndFormatReply(response);
    if(response == null)
      System.out.println("response was badly formatted");
	  System.out.println(response);
	}
 

	/*
	 * Closes an open socket.
	 *
    * @return - 0, if no error; otherwise, a negative number indicating the error
    * -1, _socket is null
    * -2, could not close socket
	 */
	public int closeSocket() 
	{
    if(_socket == null)
      return -1;


    try
    {_socket.close();}
    catch(Exception e)
    {
      System.out.println(e.toString());
      return -2;
    }
    return 0;
	}

	/**
	 * The main function. Use this function for 
	 * testing your code. We will use our own main function for testing.
	 */
	public static void main(String[] args) throws Exception
	{
    
    TCPclient client = new TCPclient();

    String message = "<reply>1.321:2.123:3.231</reply>";
    String formattedMessage = client.checkAndFormatReply(message);
    System.out.println(message + " is valid \n" + formattedMessage);

    message = "<error>1.321:2.123;3.231</error>";
    formattedMessage = client.checkAndFormatReply(message);
    System.out.println(message + " is valid \n" + formattedMessage);

    /*

    int result = 0;
    result = client.createSocket(args[0], new Integer(args[1] ));
    System.out.println(result);

    result = client.sendRequest("<echo>I'm much too big</echo>");

    System.out.println(client.receiveResponse());

    result = client.closeSocket();
    System.out.println(result);
    
    */
	}

}
