
package RoundRobinSplit_cpp;
use strict; use Cwd 'realpath';  use File::Basename;  use lib dirname(__FILE__);  use SPL::Operator::Instance::OperatorInstance; use SPL::Operator::Instance::Annotation; use SPL::Operator::Instance::Context; use SPL::Operator::Instance::Expression; use SPL::Operator::Instance::ExpressionTree; use SPL::Operator::Instance::ExpressionTreeEvaluator; use SPL::Operator::Instance::ExpressionTreeVisitor; use SPL::Operator::Instance::ExpressionTreeCppGenVisitor; use SPL::Operator::Instance::InputAttribute; use SPL::Operator::Instance::InputPort; use SPL::Operator::Instance::OutputAttribute; use SPL::Operator::Instance::OutputPort; use SPL::Operator::Instance::Parameter; use SPL::Operator::Instance::StateVariable; use SPL::Operator::Instance::TupleValue; use SPL::Operator::Instance::Window; 
sub main::generate($$) {
   my ($xml, $signature) = @_;  
   print "// $$signature\n";
   my $model = SPL::Operator::Instance::OperatorInstance->new($$xml);
   unshift @INC, dirname ($model->getContext()->getOperatorDirectory()) . "/../impl/nl/include";
   $SPL::CodeGenHelper::verboseMode = $model->getContext()->isVerboseModeOn();
   print '/* Additional includes go here */', "\n";
   print "\n";
   SPL::CodeGenHelper::implementationPrologueImpl($model,0,3);
   print "\n";
   print '// Constructor', "\n";
   print 'MY_OPERATOR_SCOPE::MY_OPERATOR::MY_OPERATOR() : _count(0)', "\n";
   print '{', "\n";
   print '    // Initialization code goes here    ', "\n";
   print '}', "\n";
   print "\n";
   print "\n";
   print '// Tuple procressing for mutating ports ', "\n";
   print 'void MY_OPERATOR_SCOPE::MY_OPERATOR::process(Tuple & tuple, uint32_t port)', "\n";
   print '{', "\n";
   print '	uint32_t const nOutputs = getNumberOfOutputPorts();', "\n";
   print '	uint32_t const batchSize = getParameter("batch");', "\n";
   print '	', "\n";
   print '	AutoPortMutex apm(_mutex, *this);', "\n";
   print '	', "\n";
   print '	uint32 outputPort = (_count / batchSize) % nOutputs;', "\n";
   print '	_count = (_count + 1) % (batchSize * nOutputs);', "\n";
   print '	assert(outputPort < nOutputs);', "\n";
   print '	submit(tuple, outputPort);', "\n";
   print '} // End of process method.', "\n";
   print "\n";
   SPL::CodeGenHelper::implementationEpilogueImpl($model, 0);
   CORE::exit $SPL::CodeGen::USER_ERROR if ($SPL::CodeGen::sawError);
}
1;
