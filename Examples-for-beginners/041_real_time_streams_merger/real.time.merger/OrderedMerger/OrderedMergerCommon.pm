package OrderedMergerCommon;

use strict;
use warnings;

sub verify($) 
{
    my ($model) = @_; 
    my $expr = $model->getParameterByName("key")->getValueAt(0);
    my $type = $expr->getSPLType();
    if (not SPL::CodeGen::Type::isOrdered($type)) {
      SPL::CodeGen::exitln("The key type (${type}) is not a numeric type",
                           $expr->getSourceLocation());
    }
    my $inputPort = $model->getInputPortAt(0);
    foreach my $iport (@{$model->getInputPorts()}) {
        if ($iport->getSPLTupleType() ne $inputPort->getSPLTupleType()) {
          SPL::CodeGen::exitln("All input ports should have the same type ",
                               $iport->getSourceLocation());
        }
    }
}

1;

