
package MyOp_cpp;
use strict; use Cwd 'realpath';  use File::Basename;  use lib dirname(__FILE__);  use SPL::Operator::Instance::OperatorInstance; use SPL::Operator::Instance::Context; use SPL::Operator::Instance::Expression; use SPL::Operator::Instance::ExpressionTree; use SPL::Operator::Instance::ExpressionTreeVisitor; use SPL::Operator::Instance::ExpressionTreeCppGenVisitor; use SPL::Operator::Instance::InputAttribute; use SPL::Operator::Instance::InputPort; use SPL::Operator::Instance::OutputAttribute; use SPL::Operator::Instance::OutputPort; use SPL::Operator::Instance::Parameter; use SPL::Operator::Instance::StateVariable; use SPL::Operator::Instance::Window; 
sub main::generate($$) {
   my ($xml, $signature) = @_;  
   print "// $$signature\n";
   my $model = SPL::Operator::Instance::OperatorInstance->new($$xml);
   unshift @INC, dirname ($model->getContext()->getOperatorDirectory()) . "/../impl/nl/include";
   $SPL::CodeGenHelper::verboseMode = $model->getContext()->isVerboseModeOn();
   print '/* Additional includes go here */', "\n";
   print "\n";
   print '/*', "\n";
   print 'This C++ primitive operator code is an example to explain the following.', "\n";
   print "\n";
   print '1) Create a source operator with a control input port.', "\n";
   print "\n";
   print '2) Receive the control stream in a control input port and use the value', "\n";
   print '   sent via the control stream in the output tuples emitted by the source thread.', "\n";
   print '   (Please refer to the operator model XML file to see how simple it is to define a ', "\n";
   print '    control port. All you have to do is to have this XML element for the control input port:', "\n";
   print '    <controlPort>true</controlPort>', "\n";
   print "\n";
   print '3) Process the operator parameter values (list of strings, integers, or floats).', "\n";
   print '   In this example, we will receive one or more string literals as an operator parameter and', "\n";
   print '   store them inside a list<rstring> member variable. Then, we will send that list<rstring>', "\n";
   print '   values as one of the attributes in the output tuples emitted by the source thread.', "\n";
   print '*/', "\n";
   print "\n";
   SPL::CodeGen::implementationPrologue($model);
   print "\n";
   print "\n";
   print '// Constructor', "\n";
   print 'MY_OPERATOR_SCOPE::MY_OPERATOR::MY_OPERATOR()', "\n";
   print '{', "\n";
       # Initialization code goes here    
       # Let us parse the operator parameter passed by the developer.
   	# Read the favoriteMovies parameter which will have one or more string literals.
   	# We will read them here and create a list<rstring>
   	# (You could pass integer or float values in a similar way.)
   	my $listFavoriteMoviesParam = $model->getParameterByName("favoriteMovies");
   	
   	if ($listFavoriteMoviesParam) {
   	    # Get the number of string literals passed via the operator parameter.
   		my $numberOfParamValues = $listFavoriteMoviesParam->getNumberOfValues();
   		print ("rstring tempStr = \"\";\n");
   		
   		# Iterate through the available number of string literals and add them to 
   		# our list<rstring> C++ member variable.
   		for (my $cnt = 0; $cnt < $numberOfParamValues; $cnt++) {
   			print ("tempStr = " . $listFavoriteMoviesParam->getValueAt($cnt)->getSPLExpression() . ";\n");
   			print ("SPL::Functions::Collections::appendM(myMovies, tempStr);\n");
   		}
   	}
   print "\n";
   print '    ', "\n";
   print '}', "\n";
   print "\n";
   print '// Destructor', "\n";
   print 'MY_OPERATOR_SCOPE::MY_OPERATOR::~MY_OPERATOR() ', "\n";
   print '{', "\n";
   print '    // Finalization code goes here', "\n";
   print '}', "\n";
   print "\n";
   print '// Notify port readiness', "\n";
   print 'void MY_OPERATOR_SCOPE::MY_OPERATOR::allPortsReady() ', "\n";
   print '{', "\n";
   print '    // Notifies that all ports are ready. No tuples should be submitted before', "\n";
   print '    // this. Source operators can use this method to spawn threads.', "\n";
   print "\n";
   print "\n";
   print '      createThreads(1); // Create source thread', "\n";
   print '}', "\n";
   print ' ', "\n";
   print '// Notify pending shutdown', "\n";
   print 'void MY_OPERATOR_SCOPE::MY_OPERATOR::prepareToShutdown() ', "\n";
   print '{', "\n";
   print '    // This is an asynchronous call', "\n";
   print '}', "\n";
   print "\n";
   print '// Processing for source and threaded operators   ', "\n";
   print 'void MY_OPERATOR_SCOPE::MY_OPERATOR::process(uint32_t idx)', "\n";
   print '{', "\n";
   print '	myName = "Jane Doe";', "\n";
   print '    myAge = 18;', "\n";
   print "\n";
   print '    // A typical implementation will loop until shutdown', "\n";
   print '      while(!getPE().getShutdownRequested()) {', "\n";
   print '          sleep(2);', "\n";
   print '          OPort0Type oTuple;', "\n";
   print '          ValueHandle handle0 = oTuple.getAttributeValue(0);', "\n";
   print '          rstring & name = handle0;', "\n";
   print '          // We will assign this attribute to a value stored in ', "\n";
   print '          // this operator\'s member variable.', "\n";
   print '          name = myName;', "\n";
   print '          ', "\n";
   print '          ValueHandle handle1 = oTuple.getAttributeValue(1);', "\n";
   print '          int32 & age = handle1;', "\n";
   print '          // We will assign this attribute to a value stored in ', "\n";
   print '          // this operator\'s member variable.', "\n";
   print '          age = myAge;', "\n";
   print '          // Increment the myAge member variable by 1.', "\n";
   print '          myAge += 1;', "\n";
   print '          ', "\n";
   print '          ValueHandle handle2 = oTuple.getAttributeValue(2);', "\n";
   print '          List & tempList = handle2;', "\n";
   print '          SPL::list<rstring> & rstringList = static_cast<SPL::list<rstring> &> (tempList);', "\n";
   print '          // We will assign this attribute to a value stored in', "\n";
   print '          // this operator\'s member variable. (This variable was', "\n";
   print '          // initialized in the operator constructor via the', "\n";
   print '          // operator params passed from the SPL operator invocation code.', "\n";
   print '          rstringList = myMovies;', "\n";
   print '          submit(oTuple, 0);', "\n";
   print '      }', "\n";
   print '}', "\n";
   print "\n";
   print '// Tuple processing for mutating ports ', "\n";
   print 'void MY_OPERATOR_SCOPE::MY_OPERATOR::process(Tuple & tuple, uint32_t port)', "\n";
   print '{', "\n";
   print ' 	// Since this input port is a control port, we should not submit anything on its', "\n";
   print ' 	// output stream while receiving the control information.', "\n";
   print '    switch(port) {', "\n";
   print '       case 0: { ', "\n";
   print '          IPort0Type & iTuple = static_cast<IPort0Type&>(tuple);', "\n";
   print '          ValueHandle handle0 = iTuple.getAttributeValue(0);', "\n";
   print '          rstring & newName = handle0;', "\n";
   print '          myName = newName;', "\n";
   print '          break;', "\n";
   print '       } ', "\n";
   print '    }', "\n";
   print '}', "\n";
   print "\n";
   print '// Tuple processing for non-mutating ports', "\n";
   print 'void MY_OPERATOR_SCOPE::MY_OPERATOR::process(Tuple const & tuple, uint32_t port)', "\n";
   print '{', "\n";
   print '    // Sample submit code', "\n";
   print '    /* ', "\n";
   print '      OPort0Type otuple;', "\n";
   print '      submit(otuple, 0); // submit to output port 0', "\n";
   print '    */', "\n";
   print '    // Sample cast code', "\n";
   print '    /*', "\n";
   print '    switch(port) {', "\n";
   print '    case 0: { ', "\n";
   print '      IPort0Type const & ituple = static_cast<IPort0Type const&>(tuple);', "\n";
   print '      ...', "\n";
   print '    } break;', "\n";
   print '    case 1: { ', "\n";
   print '      IPort1Type const & ituple = static_cast<IPort1Type const&>(tuple);', "\n";
   print '      ...', "\n";
   print '    } break;', "\n";
   print '    default: ...', "\n";
   print '    }', "\n";
   print '    */', "\n";
   print '}', "\n";
   print "\n";
   print '// Punctuation processing', "\n";
   print 'void MY_OPERATOR_SCOPE::MY_OPERATOR::process(Punctuation const & punct, uint32_t port)', "\n";
   print '{', "\n";
   print '    /*', "\n";
   print '      if(punct==Punctuation::WindowMarker) {', "\n";
   print '        // ...;', "\n";
   print '      } else if(punct==Punctuation::FinalMarker) {', "\n";
   print '        // ...;', "\n";
   print '      }', "\n";
   print '    */', "\n";
   print '}', "\n";
   print "\n";
   SPL::CodeGen::implementationEpilogue($model);
   print "\n";
   CORE::exit $SPL::CodeGen::USER_ERROR if ($SPL::CodeGen::sawError);
}
1;
