
package SimpleSourceOp_h;
use strict; use Cwd 'realpath';  use File::Basename;  use lib dirname(__FILE__);  use SPL::Operator::Instance::OperatorInstance; use SPL::Operator::Instance::Annotation; use SPL::Operator::Instance::Context; use SPL::Operator::Instance::Expression; use SPL::Operator::Instance::ExpressionTree; use SPL::Operator::Instance::ExpressionTreeEvaluator; use SPL::Operator::Instance::ExpressionTreeVisitor; use SPL::Operator::Instance::ExpressionTreeCppGenVisitor; use SPL::Operator::Instance::InputAttribute; use SPL::Operator::Instance::InputPort; use SPL::Operator::Instance::OutputAttribute; use SPL::Operator::Instance::OutputPort; use SPL::Operator::Instance::Parameter; use SPL::Operator::Instance::StateVariable; use SPL::Operator::Instance::TupleValue; use SPL::Operator::Instance::Window; 
sub main::generate($$) {
   my ($xml, $signature) = @_;  
   print "// $$signature\n";
   my $model = SPL::Operator::Instance::OperatorInstance->new($$xml);
   unshift @INC, dirname ($model->getContext()->getOperatorDirectory()) . "/../impl/nl/include";
   $SPL::CodeGenHelper::verboseMode = $model->getContext()->isVerboseModeOn();
   print '/* Additional includes go here */', "\n";
   print '#include <string>', "\n";
   print '#include <sstream>', "\n";
   print '#include <stdio.h>', "\n";
   print '#include <stdlib.h>', "\n";
   print "\n";
   print 'using namespace std;', "\n";
   print "\n";
   print '// To be able to support consistent regions, a primitive operator must have the following checks.', "\n";
      my $isInConsistentRegion = $model->getContext()->getOptionalContext("ConsistentRegion");
      my @includes;
      if ($isInConsistentRegion) {
          push @includes, "#include <SPL/Runtime/Operator/State/StateHandler.h>";
      }
      SPL::CodeGen::headerPrologue($model, \@includes);
   print "\n";
   print "\n";
   print 'class MY_OPERATOR : public MY_BASE_OPERATOR ', "\n";
   print '// To support consistent regions, this operator class must inherit from one other parent class.', "\n";
   if ($isInConsistentRegion) {
   print "\n";
   print '    , StateHandler', "\n";
   }
   print "\n";
   print '{', "\n";
   print 'public:', "\n";
   print '  // Constructor', "\n";
   print '  MY_OPERATOR();', "\n";
   print "\n";
   print '  // Destructor', "\n";
   print '  virtual ~MY_OPERATOR(); ', "\n";
   print "\n";
   print '  // Notify port readiness', "\n";
   print '  void allPortsReady(); ', "\n";
   print "\n";
   print '  // Notify pending shutdown', "\n";
   print '  void prepareToShutdown(); ', "\n";
   print "\n";
   print '  // Processing for source and threaded operators   ', "\n";
   print '  void process(uint32_t idx);', "\n";
   print '    ', "\n";
   print '  // To support consistent regions, this operator must implement these virtual member functions.  ', "\n";
   print '  ';
   if ($isInConsistentRegion) {
   print "\n";
   print '      // Implements checkpointing callbacks to persist and restore', "\n";
   print '      // operator state', "\n";
   print '      virtual void checkpoint(Checkpoint & ckpt);', "\n";
   print '      virtual void reset(Checkpoint & ckpt);', "\n";
   print '      virtual void resetToInitialState();', "\n";
   print '  ';
   }
   print "\n";
   print '  ', "\n";
   print '  ', "\n";
   print 'private:', "\n";
   print '  Mutex myMutex;', "\n";
   print '  // Product details', "\n";
   print '  int32_t serialNumber;', "\n";
   print '  int32_t productId;', "\n";
   print '  string productName; ', "\n";
   print '  int32_t quantity; ', "\n";
   print '}; ', "\n";
   print "\n";
   SPL::CodeGen::headerEpilogue($model);
   print "\n";
   print "\n";
   CORE::exit $SPL::CodeGen::USER_ERROR if ($SPL::CodeGen::sawError);
}
1;
